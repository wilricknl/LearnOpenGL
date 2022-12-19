#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>
#include <filesystem>

#include "common/camera.hpp"
#include "common/model.hpp"
#include "common/shader.hpp"

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

    auto* window = glfwCreateWindow((int)horizontal, (int)vertical, "Hello, OpenGL!", nullptr, nullptr);
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


    // grid begin
    Shader gridShader{ "grid.vert", "grid.frag" };

    std::vector<float> vertices{
        -4.0f, 0.0f,  4.0f, // 0
    	 0.0f, 0.0f,  4.0f, // 1
         4.0f, 0.0f,  4.0f, // 2

        -4.0f, 0.0f,  0.0f, // 3
    	 0.0f, 0.0f,  0.0f, // 4
         4.0f, 0.0f,  0.0f, // 5

        -4.0f, 0.0f, -4.0f, // 6
    	 0.0f, 0.0f, -4.0f, // 7
         4.0f, 0.0f, -4.0f  // 8
    };

    std::vector<unsigned int> indices{
        0, 1, 1, 2,       // top row                    - -
        0, 3, 1, 4, 2, 5, // connect top and middle    | | |
        3, 4, 4, 5,       // middle row                 - -
        3, 6, 4, 7, 5, 8, // connect middle and bottom | | |
        6, 7, 7, 8        // bottom row                 - -
    };

    unsigned int gridVao, gridVbo, gridEbo;
    glGenVertexArrays(1, &gridVao);
    glGenBuffers(1, &gridVbo);
    glGenBuffers(1, &gridEbo);
    glBindVertexArray(gridVao);

    glBindBuffer(GL_ARRAY_BUFFER, gridVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    // grid end


    stbi_set_flip_vertically_on_load(true);

    Shader shader{ "shader.vert", "shader.frag" };
    Model backpack{ "backpack/backpack.obj" };

    while (!glfwWindowShouldClose(window))
    {
        const auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        glClearColor(0.45f, 0.45f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // mvc
    	const auto projection = glm::perspective(glm::radians(camera.GetFov()), horizontal / vertical, 0.1f, 100.0f);
        const auto view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // grid
        gridShader.Use();
        gridShader.SetMat4("projection", projection);
        gridShader.SetMat4("view", view);
        gridShader.SetMat4("model", model);

    	glBindVertexArray(gridVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEbo);
        glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);

        // backpack
    	shader.Use();

        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetMat4("model", model);

        backpack.Draw(shader);

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

