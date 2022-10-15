#include "common/shader.hpp"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexShaderFile.open(vertexShaderPath);
        fragmentShaderFile.open(fragmentShaderPath);

        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    unsigned int vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    // Create vertex shader
    auto* vertexCodePtr = vertexCode.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1,& vertexCodePtr, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS,& success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    // Create fragment shader
    auto* fragmentCodePtr = fragmentCode.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1,& fragmentCodePtr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS,& success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    // Create shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS,& success);

    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
}

void Shader::Use() {
    glUseProgram(m_id);
}

void Shader::SetBool(const std::string& name, bool value) const {
    SetInt(name, (int) value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetFloat4(const std::string& name, float f1, float f2, float f3, float f4) const {
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), f1, f2, f3, f4);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::GetId() const {
    return m_id;
}
