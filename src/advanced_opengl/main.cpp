#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>
#include <filesystem>
#include <map>

#include "common/camera.hpp"
#include "common/model.hpp"
#include "common/shader.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
void mouseCallback(GLFWwindow* window, double positionX, double positionY);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

// screen
float horizontal = 800.0f;
float vertical = 600.0f;

// camera
bool bFirstMouse = true;
float lastMouseX = 400.0f;
float lastMouseY = 300.0f;
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

// timing
float deltaTime;
float lastFrame;

// outline
bool bOutline = false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow((int)horizontal, (int)vertical, "Advanced OpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00);

    stbi_set_flip_vertically_on_load(true);

    Shader shader{ "shader.vert", "shader.frag" };
    Shader shaderOutline{ "shader.vert", "outline.frag" };
    Model cube{ "cube/cube.obj" };
    Model plane{ "plane/plane.obj" };
    Model grass{ "grass/grass.obj" };
    grass.SetTextureWrap(GL_CLAMP_TO_EDGE);

    std::vector<glm::vec3> grassPositions
    {
        glm::vec3(-2.5f, 1.0f, -2.48f),
        glm::vec3(2.5f, 1.0f, 3.51f),
        glm::vec3(0.0f, 1.0f, 2.7f),
        glm::vec3(-0.3f, 1.0f, -4.3f),
        glm::vec3(5.5f, 1.0f, -0.6f)
    };

    while (!glfwWindowShouldClose(window))
    {
        const auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        glClearColor(0.45f, 0.45f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        const auto projection = glm::perspective(glm::radians(camera.GetFov()), horizontal / vertical, 0.1f, 100.0f);
        const auto view = camera.GetViewMatrix();
        plane.Draw(shader, glm::mat4(1.0f), view, projection);

        glm::mat4 model = glm::mat4(1.0f); model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		cube.Draw(shader, shaderOutline, model, view, projection, bOutline);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
        cube.Draw(shader, shaderOutline, model, view, projection, bOutline);

        std::map<float, glm::vec3> sorted;
        for (const auto& position : grassPositions)
        {
            const float distance = glm::length(camera.GetPosition() - position);
            sorted[distance] = position;
        }

        // draw furthest away first
        glDisable(GL_CULL_FACE);
        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            grass.Draw(shader, model, view, projection);
        }
        glEnable(GL_CULL_FACE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
    camera.ProcessMouseScroll(static_cast<float>(offsetY));
}

void mouseCallback(GLFWwindow* window, double positionX, double positionY)
{
    if (bFirstMouse)
    {
        lastMouseX = static_cast<float>(positionX);
        lastMouseY = static_cast<float>(positionY);
        bFirstMouse = false;
    }

    float offsetX = static_cast<float>(positionX) - lastMouseX;
    float offsetY = lastMouseY - static_cast<float>(positionY);
    lastMouseX = static_cast<float>(positionX);
    lastMouseY = static_cast<float>(positionY);

    camera.ProcessMouseMovement(offsetX, offsetY);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
        bOutline = !bOutline;
	}
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::Forward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::Backward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::Right, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::Left, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::Up, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(CameraMovement::Down, deltaTime);
    }
}
