#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

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

OGL::CameraFree freeCam(
    { 0.0f, 0.0f, 5.0f },
    { 0.0f, 0.0f, -1.0f },
    { 0.0f, 1.0f, 0.0f },
    5.0f, -90.0f, 0.0f
);

void framebufferSizeCallback
(GLFWwindow *window
    , int width
    , int height
) {
    glViewport(0, 0, width, height);
}

void processKeyInput
(GLFWwindow *window
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

unsigned int loadTexture(std::string const &path) {
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

void clearScreen
(
) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clears depth buffer
}

int main
(
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

    // Tell GLFW to capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    OGL::Shader crateShader("shaders/11-multLightsObj.vert", "shaders/11-multLightsObj.frag");
    OGL::Shader lampShader("shaders/11-multLightsLamps.vert", "shaders/11-multLightsLamps.frag");

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

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)((float)Screen::width / (float)Screen::height), 0.1f, 100.0f);

    glm::vec3 directionalLightDir{ 1.0f, -1.5f, 2.0f };
    glm::vec3 directionalLightColor{ 0.0f, 0.5f, 0.0f };

    glm::vec3 pointLightPos[2] = { {2.0f, 1.75f, 1.75f}, {-3.0f, -1.5f, -2.0f} };
    glm::vec3 pointLightColor[2] = { {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };

    OGL::DirectionalLight directionalLight{ directionalLightDir, directionalLightColor };
    OGL::PointLight pointLights[2] = { {pointLightPos[0], pointLightColor[0]}, {pointLightPos[1], pointLightColor[1]} };
    OGL::SpotLight spotLight;

    glm::mat4 model(1.0f);

    glBindVertexArray(lampVAO);
    lampShader.use();
    for (size_t i = 0; i < sizeof(pointLightPos) / sizeof(*pointLightPos); ++i) {
        glm::mat4 lampModel = glm::translate(model, pointLightPos[i]);
        lampModel = glm::scale(lampModel, glm::vec3{ 0.2f, 0.2f, 0.2f });
        lampShader.setUniformMatrix4("model[" + std::to_string(i) + "]", lampModel);
        lampShader.setUniformVec3("lampColor[" + std::to_string(i) + "]", pointLightColor[i]);
    }
    lampShader.setUniformMatrix4("projection", projection);

    glBindVertexArray(crateVAO);
    crateShader.use();

    crateShader.setUniformInt("material.textureDiffuse1", 0);
    crateShader.setUniformInt("material.textureSpecular1", 1);
    crateShader.setUniformVec3("material.colorAmbient", { 0.05f, 0.05f, 0.05f });
    crateShader.setUniformVec3("material.colorDiffuse", { 0.85f, 0.85f, 0.85f });
    crateShader.setUniformVec3("material.colorSpecular", { 0.95f, 0.95f, 0.95f });
    crateShader.setUniformFloat("material.specularExponent", 32);
    
    directionalLight.loadInShader(crateShader);

    for (size_t i = 0; i < sizeof(pointLightPos) / sizeof(*pointLightPos); ++i) {
        pointLights[i].loadInShader(crateShader, i);
    }

    spotLight.m_attenuationConst = 1.0f;
    spotLight.m_attenuationLinear = 0.007f;
    spotLight.m_attenuationQuadratic = 0.0002f;

    spotLight.loadInShader(crateShader);
    
    crateShader.setUniformMatrix4("projection", projection);

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
        for (size_t i = 0; i < sizeof(pointLightPos) / sizeof(*pointLightPos); ++i) {
            lampShader.setUniformInt("lampIndex", i);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crateDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, crateSpecular);

        glBindVertexArray(crateVAO);
        crateShader.use();
        crateShader.setUniformMatrix4("view", view);
        crateShader.setUniformVec3("viewerPos", freeCam.getPos());

        spotLight.m_position = freeCam.getPos();
        spotLight.m_direction = freeCam.getForward();
        spotLight.loadInShader(crateShader);

        for (int i = 0; i < sizeof(cubePosition) / sizeof(cubePosition[0]); ++i) {
            glm::mat4 cratePos = glm::translate(model, cubePosition[i]);
            cratePos = glm::rotate(cratePos, (float)i, glm::normalize(glm::vec3{ 1.0f, 0.3f, 0.5f }));
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