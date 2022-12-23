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
#include "stb_image.h"

unsigned int loadCubemap(std::vector<std::string> faces);
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

    auto* window = glfwCreateWindow((int)horizontal, (int)vertical, "Skybox", nullptr, nullptr);
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

    std::vector<std::string> faces
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    Shader skyboxShader{ "skybox.vert", "skybox.frag" };


    float vertices[] = {
        -1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f,  1000.0f, -1000.0f,
         1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,

        -1000.0f, -1000.0f,  1000.0f,
         1000.0f, -1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,

        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,

         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,

        -1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f,  1000.0f,
         1000.0f, -1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f, -1000.0f, -1000.0f,

        -1000.0f,  1000.0f, -1000.0f,
         1000.0f,  1000.0f, -1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f, -1000.0f
    };

    unsigned int skyboxVao, skyboxVbo;
    glGenVertexArrays(1, &skyboxVao);
    glGenBuffers(1, &skyboxVbo);
    glBindVertexArray(skyboxVao);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    while (!glfwWindowShouldClose(window))//
    {
        const auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);


        glClearColor(0.45f, 0.45f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);


        auto projection = glm::perspective(glm::radians(camera.GetFov()), horizontal / vertical, 0.1f, 100000.0f);
        auto view = camera.GetViewMatrix();

        // draw skybox
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skyboxShader.Use();
        skyboxShader.SetInt("skybox", 0);
        skyboxShader.SetMat4("projection", projection);
        skyboxShader.SetMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glBindVertexArray(skyboxVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);

        shader.Use();
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

unsigned int loadCubemap(std::vector<std::string> faces)
{
    stbi_set_flip_vertically_on_load(false);

	unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i) // might as well hardcode 6
    {
        unsigned char* data = stbi_load(faces.at(i).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cube map failed to load at path: " << faces.at(i) << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return id;
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
    static bool bWireframe = true;
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        if (bWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        bWireframe = !bWireframe;
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
