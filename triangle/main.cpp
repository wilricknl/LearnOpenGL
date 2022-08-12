#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "config.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec4 vertexColor;"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                 "  vertexColor = vec4(aColor.xyz, 1.0f);\n"
                                 "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec4 vertexColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "  FragColor = vertexColor;\n"
                                   "}\0";

const char* fragmentShaderSourceYellow = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "uniform vec4 ourColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "  FragColor = ourColor;\n"
                                         "}\0";

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

    // vertex shader creation
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << '\n';
    }

    // fragment shader creation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << '\n';
    }

    // shader program creation
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << '\n';
    }

    // fragment shader yellow creation
    unsigned int fragmentShaderYellow;
    fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, nullptr);
    glCompileShader(fragmentShaderYellow);

    glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderYellow, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT_YELLOW::COMPILATION_FAILED\n"
            << infoLog << '\n';
    }

    // shader program yellow creation
    unsigned int shaderProgramYellow;
    shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM_YELLOW::LINKING_FAILED\n"
            << infoLog << '\n';
    }
    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderYellow);

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

        glUseProgram(shaderProgram);
        glBindVertexArray(vaoLeft);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        const auto time = glfwGetTime();
        float greenValue = (sin(time) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
        glUseProgram(shaderProgramYellow);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(vaoRight);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vboLeft);
    glDeleteBuffers(1, &vboRight);
    glDeleteVertexArrays(1, &vaoLeft);
    glDeleteVertexArrays(1, &vaoRight);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramYellow);

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
