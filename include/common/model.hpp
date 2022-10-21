#ifndef LEARNOPENGL_MODEL_HPP
#define LEARNOPENGL_MODEL_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "common/mesh.hpp"
#include "common/shader.hpp"
#include "common/texture.hpp"

class Model
{
public:
    Model(const std::string& path);

    void Draw(Shader& shader);
protected:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    [[nodiscard]] Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    [[nodiscard]] std::vector<Texture> LoadMaterialTextures(
            aiMaterial* material,
            aiTextureType type,
            const std::string& typeName);
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_loadedTextures;
};

#endif //LEARNOPENGL_MODEL_HPP
