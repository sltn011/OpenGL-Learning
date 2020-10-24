#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "Mesh.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum Screen {
    width = 1000,
    height = 800
};

namespace System {
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    float lastMouseXPos = Screen::width / 2;
    float lastMouseYPos = Screen::height / 2;

    float mouseSensitivity = 0.15f;
}

OGL::CameraFree freeCam(
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 1.0f, 0.0f},
    5.0f, -90.0f, 0.0f
);

void framebufferSizeCallback
( GLFWwindow *window
, int width
, int height
) {
    glViewport(0, 0, width, height);
}

void processKeyInput
( GLFWwindow *window
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
( GLFWwindow *window
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
    glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
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
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Model Renderer", nullptr, nullptr);
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

    OGL::Shader shaderProgramm("shaders/7-texModelRender.vert", "shaders/7-texModelRender.frag");

    // Textures
    // Creating texture id
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    
    // Load texture image using stb library
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data;
    data = stbi_load("textures/dog.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        throw OGL::Exception("Error loading texture!");
    }

    // Bind texture so next texture calls use this object
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Wrap around - S = X, T = Y, Z = R
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    OGL::TexturedMesh model;
    model.readFromFile("models/dog.obj");

    auto modelData = model.getVBOData();
    size_t dataSize = model.sizeVBO();

    // Initialization code - done once(unless objects change a lot)
    // 1. Create and Bind Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 2. Creating vertex buffer - stores A LOT of vertices in GPU memory for processing
    unsigned int VBO; // Vertices Buffer Object id
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3. Creating ielement buffer to store indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 4. Copy vertices and indices to buffers
    glBufferData(GL_ARRAY_BUFFER, dataSize, modelData.get(), GL_STATIC_DRAW);

    // 5. Tell OpenGL how to interpret vertices data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OGL::TexturedVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OGL::TexturedVertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    shaderProgramm.use(); // MUST!!!
    shaderProgramm.setUniformInt("myTexture1", 0);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    shaderProgramm.use();

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)((float)Screen::width / (float)Screen::height), 0.1f, 100.0f);
    shaderProgramm.setUniformMatrix4("projection", projection, false);

    // 6. Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window);

        clearScreen();

        glm::mat4 view = freeCam.getViewMatrix();

        // After this call every render and shader function will use our shaders
        shaderProgramm.use();

        shaderProgramm.setUniformMatrix4("view", view, false);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);

        glm::mat4 modelMat(1.0f);
        modelMat = glm::scale(modelMat, {0.1f, 0.1f, 0.1f});
        modelMat = glm::rotate(modelMat, glm::radians(-90.0f), {1.0f, 0.0f, 0.0f});
        shaderProgramm.setUniformMatrix4("model", modelMat, false);

        glDrawArrays(GL_TRIANGLES, 0, model.numVertices());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();

    }

    // Free resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}