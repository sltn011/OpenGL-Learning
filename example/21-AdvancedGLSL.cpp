#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>
#include <string>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "UniformBufferObject.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int showcaseMode = 0;
int numShowcaseModes = 3;

enum Screen {
    width = 1920,
    height = 1080
};

namespace System {
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    float lastMouseXPos = Screen::width / 2;
    float lastMouseYPos = Screen::height / 2;

    float mouseSensitivity = 0.15f;
}

OGL::CameraFree freeCam{
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -1.0f},
    5.0f, 45.0f, static_cast<float>(Screen::width) / static_cast<float>(Screen::height), 1.0f, 100.0f
};

void framebufferSizeCallback( 
    GLFWwindow *window, 
    int width, 
    int height
) {
    glViewport(0, 0, width, height);
}

void processKeyInput(
    GLFWwindow *window
) {
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Camera Controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime);
    }

    double xMousePos, yMousePos;
    glfwGetCursorPos(window, &xMousePos, &yMousePos);
    float xOffset = static_cast<float>(xMousePos) - System::lastMouseXPos;
    float yOffset = System::lastMouseYPos - static_cast<float>(yMousePos);
    System::lastMouseXPos = static_cast<float>(xMousePos);
    System::lastMouseYPos = static_cast<float>(yMousePos);
    freeCam.processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
}

void keyboardInputCallback(
    GLFWwindow *window,
    int key, 
    int scancode,
    int action, 
    int mods
) {
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        showcaseMode = (showcaseMode + 1) % numShowcaseModes;
    }
}

void clearScreen(
) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(
) {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Advanced GLSL", nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetKeyCallback(window, keyboardInputCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    glViewport(0, 0, Screen::width, Screen::height);

    OGL::Shader shaderProgramm("shaders/17-advGLSL.vert", "shaders/17-advGLSL.frag");

    float vertices[] = {

        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,         
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,      
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,    

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f, 
    };

    OGL::VertexArrayObject vao;
    vao.bind();
    OGL::VertexBufferObject vbo;
    vbo.bind();
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    // UBO
    OGL::UniformBufferObject ubo;
    ubo.bind();
    ubo.allocateBufferData(2 * 64, GL_DYNAMIC_DRAW);
    ubo.setBindingPoint(1);
    shaderProgramm.uniformBlockBinding("Matrices", 1);

    shaderProgramm.use();
    shaderProgramm.setUniformMatrix4("model", glm::mat4{ 1.0f }, false);
    
    ubo.setBufferData(64, sizeof(glm::mat4), glm::value_ptr(freeCam.getProjectionMatrix()));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window);

        clearScreen();

        shaderProgramm.use();
        ubo.setBufferData(0, sizeof(glm::mat4), glm::value_ptr(freeCam.getViewMatrix()));
        shaderProgramm.setUniformInt("showcaseMode", showcaseMode);

        switch (showcaseMode) {
        case 0:
            glEnable(GL_PROGRAM_POINT_SIZE);
            glDrawArrays(GL_POINTS, 0, 36);
            glDisable(GL_PROGRAM_POINT_SIZE);
            break;

        default:
            glDrawArrays(GL_TRIANGLES, 0, 36);
            break;
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    return 0;
}