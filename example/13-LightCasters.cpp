#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

enum LightMode {
    Default,
    Directional,
    Point,
    Spot,
    SIZE
};

LightMode currentLight = LightMode::Default;

OGL::CameraFree freeCam{
    {0.0f, 0.0f, 5.0f},
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
}

void switchLightMode( 
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mode
) {
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        currentLight = (LightMode)((currentLight + 1) % LightMode::SIZE);
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

unsigned int loadTexture(
    std::string const &path
) {
    unsigned int tID;
    glGenTextures(1, &tID);

    int width, height, nrChannels;

    unsigned char *data;
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw OGL::Exception("Error loading texture!");
    }

    GLenum format;
    switch (nrChannels) {
    case 1:
        format = GL_RED;
        break;

    case 3:
        format = GL_RGB;
        break;

    case 4:
        format = GL_RGBA;
        break;

    default:
        format = 0;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, tID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    
    return tID;
}

void clearScreen(
) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Light", glfwGetPrimaryMonitor(), nullptr);
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

    glfwSetKeyCallback(window, switchLightMode);

    // Tell GLFW to capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    OGL::Shader crateShader("shaders/10-lightCasters.vert", "shaders/10-lightCasters.frag");
    OGL::Shader lampShader("shaders/9-lightBasicLamp.vert", "shaders/9-lightBasicLamp.frag");

    // Rectangle vertices
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int crateDiffuse = loadTexture("textures/Crate/crateDiffuse.png");
    unsigned int crateSpecular = loadTexture("textures/Crate/crateSpecular.png");


    unsigned int crateVAO;
    glGenVertexArrays(1, &crateVAO);
    glBindVertexArray(crateVAO);

    unsigned int crateVBO;
    glGenBuffers(1, &crateVBO);
    glBindBuffer(GL_ARRAY_BUFFER, crateVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lampVAO;
    glGenBuffers(1, &lampVAO);
    glBindVertexArray(lampVAO);

    unsigned int lampVBO;
    glGenBuffers(1, &lampVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePosition[] = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(3.0f, 6.0f, -6.0f),
        glm::vec3(-2.5f, -4.0f, -3.0f),
        glm::vec3(-4.8f, 1.0f, 1.0f),
        glm::vec3(3.4f, -1.4f, -4.5f),
        glm::vec3(2.3f, -3.0f, -3.5f),
        glm::vec3(2.5f, 3.0f, -3.5f),
        glm::vec3(2.5f, 1.2f, -2.5f),
        glm::vec3(-2.3f, 2.0f, -2.5f)
    };

    glm::vec3 lampPos{ 3.0f, 2.5f, 2.5f };
    glm::vec3 lampColor{ 1.0f, 1.0f, 1.0f };
    glm::vec3 lightDirection{ 0.0f, 0.0f, -1.0f };
    float pointLightConst = 1.0f;
    float pointLightLinear = 0.7f;
    float pointLightQuad = 1.8f;
    float cutOffAngle = 8.0f;
    float cutOffOuterAngle = 12.0f;

    glm::mat4 model(1.0f);

    glm::mat4 lampModel = glm::translate(model, lampPos);
    lampModel = glm::scale(lampModel, glm::vec3{ 0.2f, 0.2f, 0.2f });

    glBindVertexArray(lampVAO);
    lampShader.use();
    lampShader.setUniformVec3("lampColor", lampColor);
    lampShader.setUniformMatrix4("model", lampModel);
    lampShader.setUniformMatrix4("projection", freeCam.getProjectionMatrix());

    glBindVertexArray(crateVAO);
    crateShader.use();
    crateShader.setUniformVec3("light.color", lampColor);
    crateShader.setUniformVec3("light.pos", lampPos);
    crateShader.setUniformVec3("light.direction", lightDirection);
    crateShader.setUniformInt("light.mode", currentLight);
    crateShader.setUniformFloat("light.cutOff", std::cos(glm::radians(cutOffAngle)));
    crateShader.setUniformFloat("light.cutOffOuter", std::cos(glm::radians(cutOffOuterAngle)));

    crateShader.setUniformFloat("light.attConst", 1.0f);
    crateShader.setUniformFloat("light.attLinear", 0.027f);
    crateShader.setUniformFloat("light.attQuad", 0.0028f);

    crateShader.setUniformMatrix4("projection", freeCam.getProjectionMatrix());

    crateShader.setUniformInt("material.diffuseTex", 0);
    crateShader.setUniformInt("material.specularTex", 1);
    crateShader.setUniformVec3("material.ambient", { 0.05f, 0.05f, 0.05f });
    crateShader.setUniformVec3("material.diffuse", { 0.85f, 0.85f, 0.85f });
    crateShader.setUniformVec3("material.specular", { 0.95f, 0.95f, 0.95f });

    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window);

        clearScreen();

        glm::mat4 view = freeCam.getViewMatrix();

        glBindVertexArray(lampVAO);
        lampShader.use();
        lampShader.setUniformMatrix4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crateDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, crateSpecular);

        glBindVertexArray(crateVAO);
        crateShader.use();
        crateShader.setUniformInt("light.mode", currentLight);
        crateShader.setUniformMatrix4("view", view);
        crateShader.setUniformVec3("viewerPos", freeCam.getPos());
        crateShader.setUniformVec3("viewerDir", freeCam.getForward());
        for (int i = 0; i < sizeof(cubePosition) / sizeof(cubePosition[0]); ++i) {
            glm::mat4 cratePos = glm::translate(model, cubePosition[i]);
            cratePos = glm::rotate(cratePos, (float)i, glm::normalize(glm::vec3{1.0f, 0.3f, 0.5f}));
            crateShader.setUniformMatrix4("model", cratePos);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();

    }

    return 0;
}