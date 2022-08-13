#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

#include "common/shader.hpp"
#include "config.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(800, 600, "Hello, OpenGL!", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    Shader shaderProgram("shader.vert", "interpolated.frag");
    shaderProgram.use();
    shaderProgram.setFloat("horizontalOffset", 1.0f);

    Shader shaderProgramAnimated("shader.vert", "animated.frag");
    shaderProgramAnimated.use();
    shaderProgramAnimated.setFloat("horizontalOffset", -1.0f);

    float vertices[] = {
            -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.75f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.50f,  0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.75f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.50f,  0.25f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    // left triangle
    unsigned int vboLeft, vaoLeft;
    glGenVertexArrays(1, &vaoLeft);
    glGenBuffers(1, &vboLeft);
    glBindVertexArray(vaoLeft);

    glBindBuffer(GL_ARRAY_BUFFER, vboLeft);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // right triangle
    unsigned int vboRight, vaoRight;
    glGenVertexArrays(1, &vaoRight);
    glGenBuffers(1, &vboRight);
    glBindVertexArray(vaoRight);

    glBindBuffer(GL_ARRAY_BUFFER, vboRight);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices + (sizeof(vertices) / sizeof(float) / 2), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // draw wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(vaoLeft);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        const auto time = glfwGetTime();
        float greenValue = (std::sin(time) / 2.0f) + 0.5f;
        shaderProgramAnimated.use();
        shaderProgramAnimated.setFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(vaoRight);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vboLeft);
    glDeleteBuffers(1, &vboRight);
    glDeleteVertexArrays(1, &vaoLeft);
    glDeleteVertexArrays(1, &vaoRight);

    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
