#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "CameraFree.hpp"
#include "Object.hpp"

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

enum class LightMode {
    Ambient,
    Diffuse,
    Specular,
    Phong,
    SIZE
};

namespace Object {
    LightMode currentMode = LightMode::Phong;

    float ambientStr = 0.3f;
    float ambientDelta = 0.01f;

    float specularStr = 0.5f;
    float specularDelta = 0.01f;

    int shininess = 32;
    int shininessDelta = 1;
}


OGL::CameraFree freeCam{
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 1.0f, 0.0f},
    5.0f, -90.0f, 0.0f
};

void windowResizeCallback
(GLFWwindow *window
    , int newWidth
    , int newHeight
) {
    glViewport(0, 0, newWidth, newHeight);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main() {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Model Renderer", glfwGetPrimaryMonitor(), nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouseCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OGL::Exception("Error loading GLAD!");
    }

    stbi_set_flip_vertically_on_load(true);

    OGL::Shader shader("shaders/0-renderer.vert", "shaders/0-renderer.frag");

    OGL::Model myModel("models/Backpack/backpack.obj");

    OGL::Object myObject(myModel);
    myObject.m_postiton = glm::vec3{ 0.0f, 0.0f, -2.0f };
    myObject.m_scale = 1.2f;
    myObject.m_rotationAngleRadians = glm::radians(-45.0f);
    myObject.m_rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 lightPos{ 3.0f, 2.5f, 2.0f };
    glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };

    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Screen::width / (float)Screen::height, 0.1f, 100.0f);
    shader.setUniformMatrix4("projection", projection, false);

    while (!glfwWindowShouldClose(window)) {
        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        clearScreen();

        shader.use();
        glm::mat4 view = freeCam.getViewMatrix();
        
        shader.setUniformMatrix4("view", view, false);
        
        shader.setUniformVec3("lightPos", lightPos);
        shader.setUniformVec3("lightColor", lightColor);
        shader.setUniformVec3("viewerPos", freeCam.getPos());

        myObject.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}