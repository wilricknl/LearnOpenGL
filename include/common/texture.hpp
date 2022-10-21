#ifndef LEARNOPENGL_TEXTURE_HPP
#define LEARNOPENGL_TEXTURE_HPP

#include <string>

class Texture
{
public:
    Texture() = default;
    Texture(unsigned int id, std::string type);

    void FromFile(const std::string& filename, const std::string& directory, bool bGamma = false);

    [[nodiscard]] unsigned int GetId() const;
    void SetId(unsigned  int id);

    [[nodiscard]] const std::string& GetType() const;
    void SetType(std::string type);

    [[nodiscard]] const std::string& GetFilename() const;
    void SetFilename(std::string filename);
private:
    unsigned int m_id;
    std::string m_type;
    std::string m_filename;
};

#endif //LEARNOPENGL_TEXTURE_HPP
