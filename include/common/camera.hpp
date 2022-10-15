#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class CameraMovement
{
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class Camera
{
public:
    static constexpr float DefaultYaw = -90.0f;
    static constexpr float DefaultPitch = 0.0f;
    static constexpr float DefaultSpeed = 2.5f;
    static constexpr float DefaultSensitivity = 0.1f;
    static constexpr float DefaultFov = 45.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = DefaultYaw,
           float pitch = DefaultPitch,
           glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
           float speed = DefaultSpeed,
           float sensitivity = DefaultSensitivity,
           float fov = DefaultFov);

    Camera(float positionX, float positionY, float positionZ,
           float upX, float upY, float upZ,
           float yaw = DefaultYaw, float pitch = DefaultPitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMovement movement, float deltaTime);

    void ProcessMouseMovement(float offsetX, float offsetY, GLboolean bPitchClamp = true);

    void ProcessMouseScroll(float offsetY);
protected:
    void Update();
public:
    [[nodiscard]] const glm::vec3& GetPosition() const;
    void SetPosition(const glm::vec3& position);

    [[nodiscard]] const glm::vec3& GetFront() const;

    [[nodiscard]] const glm::vec3& GetUp() const;

    [[nodiscard]] const glm::vec3& GetRight() const;

    [[nodiscard]] const glm::vec3& GetWorldUp() const;
    void SetWorldUp(const glm::vec3& worldUp);

    [[nodiscard]] float GetYaw() const;
    void SetYaw(float yaw);

    [[nodiscard]] float GetPitch() const;
    void SetPitch(float pitch);

    [[nodiscard]] float GetSpeed() const;
    void SetSpeed(float speed);

    [[nodiscard]] float GetSensitivity() const;
    void SetSensitivity(float sensitivity);

    [[nodiscard]] float GetFov() const;
    void SetFov(float fov);
private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    float m_yaw;
    float m_pitch;
    float m_speed;
    float m_sensitivity;
    float m_fov;
};

#endif // CAMERA_HPP
