#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "Object.hpp"
#include "PointLight.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

OGL::Shader *ShaderPtr = nullptr;
bool PerFragmentNormals = false;

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
    {0.20f, 1.0f, 1.0f},
    {0.0f, 0.0f, -1.0f},
    5.0f, 45.0f, static_cast<float>(Screen::width) / static_cast<float>(Screen::height), 0.01f, 100.0f
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
}

void mouseCallback(
    GLFWwindow *window,
    double xpos,
    double ypos
) {
    float xOffset = static_cast<float>(xpos) - System::lastMouseXPos;
    float yOffset = System::lastMouseYPos - static_cast<float>(ypos);
    System::lastMouseXPos = static_cast<float>(xpos);
    System::lastMouseYPos = static_cast<float>(ypos);

    freeCam.processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
}

void keyboardCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (ShaderPtr) {
            PerFragmentNormals ^= true;
            ShaderPtr->use();
            ShaderPtr->setUniformBool("PerFragmentNormals", PerFragmentNormals);
        }
    }
}

void clearScreen(
) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clears depth buffer
}

int main(
) {
    // Initialize GLFW
    OGL::GLFWInitRAII glfwInitializer;

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Hello, 3D", nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 2;
    }

    // Set window as current context
    glfwMakeContextCurrent(window);

    // Set callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Set callback for mouse movement events
    glfwSetCursorPosCallback(window, mouseCallback);

    // Set keyboard callback
    glfwSetKeyCallback(window, keyboardCallback);

    // Tell GLFW to capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    glEnable(GL_DEPTH_TEST);

    OGL::Shader shaderProgramm("shaders/24-normalMapping.vert", "shaders/24-normalMapping.frag");
    shaderProgramm.showWarnings(true);
    ShaderPtr = &shaderProgramm;

    OGL::Model WallModel("models/BrickWall/Var1/brickWall.obj");
    OGL::Object Wall(&WallModel, glm::vec3{ 1.0f, 0.0f, 1.0f }, 1.0f, glm::vec3{ -90.0f, 0.0f, 0.0f });

    OGL::PointLight Light(glm::vec3{ 4.0f, 2.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });

    shaderProgramm.use();
    Light.loadInShader(shaderProgramm, 0);
    shaderProgramm.setUniformInt("numPointLights", 1);
    shaderProgramm.setUniformMatrix4("projection", freeCam.getProjectionMatrix());
    shaderProgramm.setUniformBool("PerFragmentNormals", PerFragmentNormals);

    // 6. Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        glm::vec3 pos = Wall.getPosition();
        float x = pos.x;
        float z = pos.z;
        float newX = x * cos(System::deltaTime) - z * sin(System::deltaTime);
        float newZ = z * cos(System::deltaTime) + x * sin(System::deltaTime);
        pos.x = newX;
        pos.z = newZ;
        //Wall.setPosition(pos);

        processKeyInput(window);

        clearScreen();

        shaderProgramm.use();
        shaderProgramm.setUniformVec3("viewerPos", freeCam.getPos());
        shaderProgramm.setUniformMatrix4("view", freeCam.getViewMatrix());
        Wall.draw(shaderProgramm);
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();

    }

    return 0;
}