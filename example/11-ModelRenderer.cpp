#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "CameraFree.hpp"

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

int main() {
    // Initialize GLFW
    OGL::GLFWInitRAII glfwInitializer;

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Model Renderer", nullptr, nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
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
        throw OGL::Exception("Error initializing GLAD!");
    }

    OGL::Shader shaderProgramm("shaders/6-ModelRenderer.vert", "shaders/6-ModelRenderer.frag");

    OGL::ColoredMesh model;
    if (!model.readFromFile("models/teapot.ply")) {
        throw OGL::Exception("Error loading model from .ply file!");
    }

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

    auto VBOData = model.getVBOData();
    size_t vboSize = model.sizeVBO();

    auto EBOData = model.getEBOData();
    size_t eboSize = model.sizeEBO();

    glBufferData(GL_ARRAY_BUFFER, vboSize, VBOData.get(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, EBOData.get(), GL_STATIC_DRAW);

    // 0 - Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(OGL::RGBA), (void*)0);

    // 1 - Texture
    //

    // 2 - Color
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(OGL::RGBA), (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    shaderProgramm.use();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)((float)Screen::width / (float)Screen::height), 0.1f, 100.0f);
    shaderProgramm.setUniformMatrix4("projection", projection, false);

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
        
        glBindVertexArray(VAO);

        glm::mat4 modelMatr(1.0f);
        modelMatr = glm::rotate(modelMatr, glm::radians(-90.0f), {1.0f, 0.0f, 0.0f});
        shaderProgramm.setUniformMatrix4("model", modelMatr, false);

        glDrawElements(GL_TRIANGLES, model.numElements(), GL_UNSIGNED_INT, 0);

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