#ifndef LEARNOPENGL_MODEL_HPP
#define LEARNOPENGL_MODEL_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "common/mesh.hpp"
#include "common/shader.hpp"
#include "common/texture.hpp"

class Model
{
public:
    Model(const std::string& path);

    [[deprecated]] void Draw(Shader& shader);
    void Draw(
        Shader& shader, 
		const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection);
    void Draw(
        Shader& shader,
        Shader& outline,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection,
        bool bOutline);
    void DrawOutline(
        Shader& shader, 
        Shader& outline,
        glm::mat4 model,
        const glm::mat4& view,
        const glm::mat4& projection);
protected:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    [[nodiscard]] Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    [[nodiscard]] std::vector<Texture> LoadMaterialTextures(
            aiMaterial* material,
            aiTextureType type,
            const std::string& typeName);
public:
    float GetOutlineScale() const;
    void SetOutlineScale(float outlineScale);
private:
    float m_outlineScale;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_loadedTextures;
};

#endif //LEARNOPENGL_MODEL_HPP
