#include "common/model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

Model::Model(const std::string& path)
    : m_outlineScale(1.05f), m_meshes(), m_directory()
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


void Model::Draw(
    Shader& shader,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection)
{
    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    for (auto& mesh : m_meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::Draw(
    Shader& shader,
    Shader& outline,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection,
    bool bOutline)
{
    if (bOutline)
    {
        DrawOutline(shader, outline, model, view, projection);
    }
    else
    {
        Draw(shader, model, view, projection);
    }
}

void Model::DrawOutline(
    Shader& shader, 
    Shader& outline,
    glm::mat4 model,
    const glm::mat4& view,
    const glm::mat4& projection)
{
    // todo: note for future me, uncomment this if you don't want to combine outlines
    // glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    Draw(shader, model, view, projection);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    model = glm::scale(model, glm::vec3(m_outlineScale));
    Draw(outline, model, view, projection);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void Model::SetTextureWrap(int wrap) const
{
	for (auto& texture : m_loadedTextures)
	{
        glBindTexture(GL_TEXTURE_2D, texture.GetId());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glBindTexture(GL_TEXTURE_2D, 0);
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

float Model::GetOutlineScale() const
{
    return m_outlineScale;
}

void Model::SetOutlineScale(float outlineScale)
{
    m_outlineScale = outlineScale;
}
