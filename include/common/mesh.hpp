#ifndef LEARNOPENGL_MESH_HPP
#define LEARNOPENGL_MESH_HPP

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "common/texture.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);

    void Draw(class Shader& shader);

protected:
    void SetupMesh();

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
};

#endif //LEARNOPENGL_MESH_HPP
