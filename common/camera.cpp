#include "common/camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position,
        glm::vec3 up,
        float yaw,
        float pitch,
        glm::vec3 front,
        float speed,
        float sensitivity,
        float fov)
        : m_position(position),
        m_front(front),
        m_up(up),
        m_right(),
        m_worldUp(up),
        m_yaw(yaw),
        m_pitch(pitch),
        m_speed(speed),
        m_sensitivity(sensitivity),
        m_fov(fov)
{
    Update();
}

Camera::Camera(float positionX, float positionY, float positionZ,
       float upX, float upY, float upZ,
       float yaw, float pitch)
       : Camera({positionX, positionY, positionZ},
                {upX, upY, upZ},
                yaw,
                pitch)
{}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessKeyboard(CameraMovement movement, float deltaTime)
{
    float velocity = m_speed * deltaTime;
    switch(movement)
    {
        case CameraMovement::Forward:
            m_position += m_front * velocity;
            break;
        case CameraMovement::Backward:
            m_position -= m_front * velocity;
            break;
        case CameraMovement::Right:
            m_position += m_right * velocity;
            break;
        case CameraMovement::Left:
            m_position -= m_right * velocity;
            break;
	    case CameraMovement::Up:
            m_position += m_up * velocity;
	        break;
	    case CameraMovement::Down:
            m_position -= m_up * velocity;
	        break;
    }
}

void Camera::ProcessMouseMovement(float offsetX, float offsetY, GLboolean bPitchClamp)
{
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;
    m_yaw += offsetX;
    m_pitch += offsetY;

    if (bPitchClamp)
    {
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }

    Update();
}

void Camera::ProcessMouseScroll(float offsetY)
{
    m_fov = std::clamp(m_fov - offsetY, 1.0f, 45.0f);
}

void Camera::Update()
{
    glm::vec3 front;
    front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    front.y = std::sin(glm::radians(m_pitch));
    front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

const glm::vec3& Camera::GetPosition() const
{
    return m_position;
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

const glm::vec3& Camera::GetFront() const
{
    return m_front;
}

const glm::vec3& Camera::GetUp() const
{
    return m_up;
}

const glm::vec3& Camera::GetRight() const
{
    return m_right;
}

const glm::vec3& Camera::GetWorldUp() const
{
    return m_worldUp;
}

void Camera::SetWorldUp(const glm::vec3& worldUp)
{
    m_worldUp = worldUp;
}

float Camera::GetYaw() const
{
    return m_yaw;
}

void Camera::SetYaw(float yaw)
{
    m_yaw = yaw;
}

float Camera::GetPitch() const
{
    return m_pitch;
}

void Camera::SetPitch(float pitch)
{
    m_pitch = pitch;
}

float Camera::GetSpeed() const
{
    return m_speed;
}

void Camera::SetSpeed(float speed)
{
    m_speed = speed;
}

float Camera::GetSensitivity() const
{
    return m_sensitivity;
}

void Camera::SetSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

float Camera::GetFov() const
{
    return m_fov;
}

void Camera::SetFov(float fov)
{
    m_fov = fov;
}