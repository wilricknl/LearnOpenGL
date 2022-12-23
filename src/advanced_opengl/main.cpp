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


	// framebuffer
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // framebuffer texture
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)horizontal, (int)vertical, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // render buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)horizontal, (int)vertical);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach render buffer to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check framebuffer correctness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER::INCOMPLETE" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // quad to render to
    Shader quadShader{ "quad.vert", "quad.frag" };

    float quadVertices[]
    {
        -1.0f,  1.0f, 0.0f, 1.0f, // left top
        -1.0f, -1.0f, 0.0f, 0.0f, // left bottom
         1.0f, -1.0f, 1.0f, 0.0f, // right bottom

        -1.0f,  1.0f, 0.0f, 1.0f, // left top
         1.0f, -1.0f, 1.0f, 0.0f, // right bottom
         1.0f,  1.0f, 1.0f, 1.0f, // right top
    };

    unsigned int quadVao, quadVbo;
    glGenVertexArrays(1, &quadVao);
    glGenBuffers(1, &quadVbo);
    glBindVertexArray(quadVao);

    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    // framebuffer end


    // mirror
    float mirrorVertices[]
    {
        -0.75f,  1.0f,  0.0f, 1.0f, // left top
        -0.75f,  0.75f, 0.0f, 0.0f, // left bottom
         0.75f,  0.75f, 1.0f, 0.0f, // right bottom

        -0.75f,  1.0f,  0.0f, 1.0f, // left top
         0.75f,  0.75f, 1.0f, 0.0f, // right bottom
         0.75f,  1.0f,  1.0f, 1.0f, // right top
    };

    unsigned int mirrorVao, mirrorVbo;
    glGenVertexArrays(1, &mirrorVao);
    glGenBuffers(1, &mirrorVbo);
    glBindVertexArray(mirrorVao);

    glBindBuffer(GL_ARRAY_BUFFER, mirrorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(mirrorVertices), &mirrorVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));

    // mirror end


	while (!glfwWindowShouldClose(window))
    {
        const auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);


    	// first pass with custom frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.45f, 0.45f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        auto projection = glm::perspective(glm::radians(camera.GetFov()), horizontal / vertical, 0.1f, 100.0f);
        auto view = camera.GetViewMatrix();
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


    	// second pass with default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        quadShader.Use();
        glBindVertexArray(quadVao);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // third pass with custom frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.45f, 0.45f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        projection = glm::perspective(glm::radians(camera.GetFov()), (0.75f * horizontal) / (vertical * 0.125f), 0.1f, 100.0f);
        projection = glm::rotate(projection, glm::radians(180.0f), glm::vec3{0.0f, 1.0f, 0.0f });
        view = camera.GetViewMatrix();
        plane.Draw(shader, glm::mat4(1.0f), view, projection);

        model = glm::mat4(1.0f); model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        cube.Draw(shader, shaderOutline, model, view, projection, bOutline);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
        cube.Draw(shader, shaderOutline, model, view, projection, bOutline);

        sorted.clear();
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

        // fourth pass with default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

		quadShader.Use();
        glBindVertexArray(mirrorVao);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);



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
