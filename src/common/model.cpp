#include "common/model.hpp"

#include "stb_image.h"

Model::Model(const std::string& path)
    : m_meshes(), m_directory()
{
    LoadModel(path);
}

void Model::Draw(Shader& shader)
{
    for (auto& mesh : m_meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const auto* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }

    m_directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        auto* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned  int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex{};
        glm::vec3 vector{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        vertex.position = vector;

        vector = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 coordinates;
            coordinates.x = mesh->mTextureCoords[0][i].x;
            coordinates.y = mesh->mTextureCoords[0][i].y;
            vertex.textureCoordinates = coordinates;
        }
        else
        {
            vertex.textureCoordinates = glm::vec2{ 0.0f, 0.0f };
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        auto face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        auto* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh{ vertices, indices, textures };
}

std::vector<Texture> Model::LoadMaterialTextures(
        aiMaterial* material,
        aiTextureType type,
        const std::string& typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString string;
        material->GetTexture(type, i, &string);
        std::string filename{ string.C_Str() };

        bool bSkip = false;
        for (auto& texture : m_loadedTextures)
        {
            if (filename == texture.GetFilename())
            {
                textures.push_back(texture);
                bSkip = true;
                break;
            }
        }

        if (!bSkip)
        {
            Texture texture;
            texture.FromFile(filename, m_directory);
            texture.SetType(typeName);
            textures.push_back(texture);
            m_loadedTextures.push_back(texture);
        }
    }

    return textures;
}
