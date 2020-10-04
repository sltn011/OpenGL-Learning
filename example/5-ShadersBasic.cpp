#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"

enum Screen {
    width = 1000,
    height = 800
};

void framebufferSizeCallback
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
}

void clearScreen
(
) {
    glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Hello, Triangle", nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 2;
    }

    // Set window as current context
    glfwMakeContextCurrent(window);

    // Set callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    // Set dimensions of rendering window - callback gets called when window first time created
    //glViewport(0, 0, Screen::width, Screen::height)

    OGL::Shader shaderProgramm("shaders/1-vertex.txt", "shaders/1-fragment.txt");

    // Triangle vertices
    float trVertices[] = {
        // Position             // Colors
        -0.5f, -0.5f, +0.0f,    1.0f, 0.0f, 0.0f,
        +0.5f, -0.5f, +0.0f,    0.0f, 1.0f, 0.0f,
        +0.0f, +0.5f, +0.0f,    0.0f, 0.0f, 1.0f
    };

    // Initialization code - done once(unless objects change a lot)
    // 1. Create and Bind Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 2. Creating vertex buffer - stores A LOT of vertices in GPU memory for processing
    unsigned int VBO; // Vertices Buffer Object id
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3. Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(trVertices), trVertices, GL_STATIC_DRAW);

    // 4. Tell OpenGL how to interpret vertices data
    // @Vertex attribute location(index?)
    // @Size of vertext attribute(vec3 is 3 values)
    // @Data type
    // @Should be normalized?
    // @Stride
    // @Offset

    // Position of vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color of vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 5. Render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        clearScreen();

        shaderProgramm.use(); // use shader prog before updating uniform

        //float time = glfwGetTime();
        //float redValue = std::sin(time) / 2.0f + 0.5f;
        //float blueValue = std::cos(time) / 2.0f + 0.5f;
        //int vertexColorLoc = glGetUniformLocation(shaderProgrammId, "vertexColor");
        //glUniform4f(vertexColorLoc, redValue, 0.0f, blueValue, 1.0f);

        glBindVertexArray(VAO);

        // @Starting index of vertices array
        // @How many vertices to draw 
        glDrawArrays(GL_TRIANGLES, 0, 3);        
        
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