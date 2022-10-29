#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>

#include "common/camera.hpp"
#include "common/shader.hpp"
#include "config.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
void mouseCallback(GLFWwindow* window, double positionX, double positionY);
void processInput(GLFWwindow* window);

// screen
float horizontal = 800.0f;
float vertical = 600.0f;

// camera
bool bFirstMouse = true;
float lastMouseX = 400.0f;
float lastMouseY = 300.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// timing
float deltaTime;
float lastFrame;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow((int)horizontal, (int)vertical, "lighting", nullptr, nullptr);
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader cubeShader("shader.vert", "shader.frag");
    Shader lightShader("light.vert", "light.frag");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    constexpr int cubePositionsSize = 10;
    glm::vec3 cubePositions[cubePositionsSize] {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    constexpr int pointLightPositionsSize = 4;
    glm::vec3 pointLightPositions[pointLightPositionsSize] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    unsigned int vbo, cubeVao;
    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(cubeVao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // diffuse map loading
    unsigned int diffuseMap;
    glGenTextures(1, &diffuseMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load 'container2.png'!\n";
    }
    stbi_image_free(data);

    // specular map loading
    unsigned int specularMap;
    glGenTextures(1, &specularMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load 'container2_specular.png'!\n";
    }
    stbi_image_free(data);

    // light vao
    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        const auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeShader.Use();
        // directional light
        cubeShader.SetVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
        cubeShader.SetVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
        cubeShader.SetVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        cubeShader.SetVec3("pointLights[0].position", pointLightPositions[0]);
        cubeShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[0].constant", 1.0f);
        cubeShader.SetFloat("pointLights[0].linear", 0.09f);
        cubeShader.SetFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        cubeShader.SetVec3("pointLights[1].position", pointLightPositions[1]);
        cubeShader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[1].diffuse", 0.0f, 0.0f, 1.0f);
        cubeShader.SetVec3("pointLights[1].specular", 0.0f, 0.0f, 1.0f);
        cubeShader.SetFloat("pointLights[1].constant", 1.0f);
        cubeShader.SetFloat("pointLights[1].linear", 0.09f);
        cubeShader.SetFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        cubeShader.SetVec3("pointLights[2].position", pointLightPositions[2]);
        cubeShader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[2].constant", 1.0f);
        cubeShader.SetFloat("pointLights[2].linear", 0.09f);
        cubeShader.SetFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        cubeShader.SetVec3("pointLights[3].position", pointLightPositions[3]);
        cubeShader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[3].constant", 1.0f);
        cubeShader.SetFloat("pointLights[3].linear", 0.09f);
        cubeShader.SetFloat("pointLights[3].quadratic", 0.032f);
        // spot light
        cubeShader.SetVec3("spotLight.position", camera.GetPosition());
        cubeShader.SetVec3("spotLight.direction", camera.GetFront());
        cubeShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cubeShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("spotLight.constant", 1.0f);
        cubeShader.SetFloat("spotLight.linear", 0.09f);
        cubeShader.SetFloat("spotLight.quadratic", 0.032f);
        cubeShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        // material
        cubeShader.SetVec3("viewPosition", camera.GetPosition());
        cubeShader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        cubeShader.SetInt("material.diffuse", 0);
        cubeShader.SetInt("material.specular", 1);
        cubeShader.SetFloat("material.shininess", 32.0f);

        const auto projection = glm::perspective(glm::radians(camera.GetFov()), horizontal / vertical, 0.1f, 100.0f);
        const auto view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);
        cubeShader.SetMat4("projection", projection);
        cubeShader.SetMat4("view", view);

        glBindVertexArray(cubeVao);
        for (size_t i = 0; i < cubePositionsSize; ++i)
        {
            model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // draw light bulbs
        lightShader.Use();
        lightShader.SetMat4("projection", projection);
        lightShader.SetMat4("view", view);
        for (size_t i = 0; i < pointLightPositionsSize; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.SetMat4("model", model);

            glBindVertexArray(lightVao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVao);
    glDeleteVertexArrays(1, &lightVao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(cubeShader.GetId());
    glDeleteProgram(lightShader.GetId());

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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
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
