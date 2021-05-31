#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    5.0f, 45.0f, static_cast<float>(Screen::width) / static_cast<float>(Screen::height), 0.01f, 100.0f
};

void windowResizeCallback(
    GLFWwindow *window,
    int width,
    int height
) {
    glViewport(0, 0, width, height);
}

void processInput( 
    GLFWwindow *window
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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Object::ambientStr = Object::ambientStr + Object::ambientDelta;
        if (Object::ambientStr > 1.0f) {
            Object::ambientStr = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Object::ambientStr = Object::ambientStr - Object::ambientDelta;
        if (Object::ambientStr < 0.0f) {
            Object::ambientStr = 0.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Object::specularStr += Object::specularDelta;
        if (Object::specularStr > 1.0f) {
            Object::specularStr = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Object::specularStr -= Object::specularDelta;
        if (Object::specularStr < 0.0f) {
            Object::specularStr = 0.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Object::shininess += Object::shininessDelta;
        if (Object::shininess > 256) {
            Object::shininess = 256;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Object::shininess -= Object::shininessDelta;
        if (Object::shininess < 1) {
            Object::shininess = 1;
        }
    }
}

void lightModeSwitch(
    GLFWwindow *window,
    int key,
    int scancode,
    int action, 
    int mode
) {
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        Object::currentMode = (LightMode)(((int)Object::currentMode + 1) % (int)LightMode::SIZE);
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

void clearScreen(
) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(
) {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Light", glfwGetPrimaryMonitor(), nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetKeyCallback(window, lightModeSwitch);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OGL::Exception("Error loading GLAD!");
    }

    OGL::Shader objectShader("shaders/9-lightBasicObject.vert", "shaders/9-lightBasicObject.frag");
    OGL::Shader lampShader("shaders/9-lightBasicLamp.vert", "shaders/9-lightBasicLamp.frag");

    float cubeVertices[] = {
        // Coord              // Normal
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    unsigned int objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    unsigned int lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 objectColor{ 0.35f, 0.2f, 0.4f };
    glm::vec3 lampColor{ 1.0f, 1.0f, 1.0f };

    glm::mat4 model{1.0f};

    glm::mat4 objModel = glm::translate(model, { 0.0f, 0.0f, -5.0f });

    objectShader.use();
    objectShader.setUniformVec3("objectColor", objectColor);
    objectShader.setUniformVec3("lampColor", lampColor);
    objectShader.setUniformMatrix4("model", objModel, false);
    objectShader.setUniformMatrix4("projection", freeCam.getProjectionMatrix(), false);

    lampShader.use();
    lampShader.setUniformVec3("lampColor", lampColor);
    lampShader.setUniformMatrix4("projection", freeCam.getProjectionMatrix(), false);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processInput(window);
        clearScreen();

        glm::mat4 view = freeCam.getViewMatrix();

        glm::vec3 lampPos = { 0.0f, 1.0f, -5.0f };
        lampPos.x = sin((float)glfwGetTime()) * 1.5f + lampPos.x;
        lampPos.z = cos((float)glfwGetTime()) * 1.5f + lampPos.z;
        glm::mat4 lampModel = glm::translate(model, lampPos);
        lampModel = glm::scale(lampModel, glm::vec3{ 0.20f });

        objectShader.use();

        objectShader.setUniformInt("lightMode", (int)Object::currentMode);
        objectShader.setUniformFloat("ambientStr", Object::ambientStr);
        objectShader.setUniformFloat("specularStr", Object::specularStr);
        objectShader.setUniformInt("shininess", Object::shininess);
        objectShader.setUniformVec3("lampPos", lampPos);
        objectShader.setUniformVec3("viewerPos", freeCam.getPos());

        objectShader.setUniformMatrix4("view", view, false);
        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampShader.use();

        lampShader.setUniformMatrix4("view", view, false);
        lampShader.setUniformMatrix4("model", lampModel, false);
        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &VBO);
}