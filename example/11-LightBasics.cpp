#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

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

OGL::CameraFree freeCam(
    { 0.0f, 0.0f, 5.0f },
    { 0.0f, 0.0f, -1.0f },
    { 0.0f, 1.0f, 0.0f },
    5.0f, -90.0f, 0.0f
);

void framebufferResizeCallback
( GLFWwindow *window
, int width
, int height
) {
    glViewport(0, 0, width, height);
}

void processInput
( GLFWwindow *window
) {
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

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
}

void mouseCallback
(GLFWwindow *window
, double xpos
, double ypos
) {
    float xOffset = static_cast<float>(xpos) - System::lastMouseXPos;
    float yOffset = System::lastMouseYPos - static_cast<float>(ypos);
    System::lastMouseXPos = static_cast<float>(xpos);
    System::lastMouseYPos = static_cast<float>(ypos);

    freeCam.processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
}

void clearScreen
(
) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main() {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Light", glfwGetPrimaryMonitor(), nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OGL::Exception("Error initializing GLAD");
    }

    OGL::Shader shaderObject("shaders/8-lightBasicObject.vert", "shaders/8-lightBasicObject.frag");
    OGL::Shader shaderLamp("shaders/8-lightBasicLamp.vert", "shaders/8-lightBasicLamp.frag");

    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
    };

    int cubeIndices[] = {
        0, 1, 2,
        0, 2, 3,
        3, 2, 6,
        3, 6, 7,
        7, 6, 5,
        7, 5, 4,
        4, 5, 1,
        4, 1, 0,
        1, 5, 6,
        1, 6, 2,
        4, 0, 3,
        4, 3, 7,
    };

    // Object
    unsigned int objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    unsigned int objectVBO;
    glGenBuffers(1, &objectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, objectVBO);

    unsigned int objectEBO;
    glGenBuffers(1, &objectEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectEBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // Lamp
    unsigned int lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);

    unsigned int lampVBO;
    glGenBuffers(1, &lampVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);

    unsigned int lampEBO;
    glGenBuffers(1, &lampEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);


    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processInput(window);
        clearScreen();


        glm::vec3 lampColor{ std::abs(std::sin((float)glfwGetTime())), 1.0f, 1.0f };
        glm::vec3 objectColor{ 0.7f, 0.35f, 0.2f };

        glm::mat4 view = freeCam.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)(Screen::width) / (float)(Screen::height)), 0.1f, 100.0f);

        shaderObject.use();
        shaderObject.setUniformVec3("objectColor", objectColor);
        shaderObject.setUniformVec3("lampColor", lampColor);
        shaderObject.setUniformMatrix4("view", view, false);
        glm::mat4 objModelMat = glm::translate(glm::mat4{ 1.0f }, { 0.0f, 0.0f, -4.0f });
        shaderObject.setUniformMatrix4("model", objModelMat, false);
        shaderObject.setUniformMatrix4("projection", projection, false);
        
        glBindVertexArray(objectVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        shaderLamp.use();
        shaderLamp.setUniformVec3("lampColor", lampColor);
        shaderLamp.setUniformMatrix4("view", view, false);
        glm::mat4 lampModelMat = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.25 });
        lampModelMat = glm::translate(lampModelMat, { -1.5f, -1.5f, -3.0f });
        shaderLamp.setUniformMatrix4("model", lampModelMat, false);
        shaderLamp.setUniformMatrix4("projection", projection, false);
        
        glBindVertexArray(lampVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &objectVAO);
    glDeleteBuffers(1, &objectVBO);
    glDeleteBuffers(1, &objectEBO);

    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);
    glDeleteBuffers(1, &lampEBO);
}