#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

enum Screen {
    width = 1000,
    height = 800
};

namespace Shaders {
    char const *vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}"
    ;

    char const *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}"
    ;
}

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

unsigned int compileGLShader
( char const *sourceCode
, int shaderType
) {
    unsigned int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &sourceCode, nullptr);
    glCompileShader(shaderId);
    return shaderId;
}

bool shaderCorrectlyCompiled
( unsigned int shaderId
) {
    int correctness;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &correctness);
    return correctness;
}

void reportShaderCompileError
( unsigned int shaderId
) {
    char infoLog[512];
    glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog);
    std::cerr << infoLog << std::endl;
}

bool programmCorrectlyLinked
( unsigned int programmId
) {
    int correctness;
    glGetProgramiv(programmId, GL_LINK_STATUS, &correctness);
    return correctness;
}

void reportProgrammLinkError
( unsigned int programmId
) {
    char infoLog[512];
    glGetProgramInfoLog(programmId, sizeof(infoLog), nullptr, infoLog);
    std::cerr << infoLog << std::endl;
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
    if(!glfwInit()) {
        return 1;
    }

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

    // Compile Vertex Shader
    unsigned int vertexShaderId = compileGLShader(Shaders::vertexShaderSource, GL_VERTEX_SHADER);
    if (!shaderCorrectlyCompiled(vertexShaderId)) {
        reportShaderCompileError(vertexShaderId);
        glfwTerminate();
        return 4;
    }

    // Compile Fragment Shader
    unsigned int fragmentShaderId = compileGLShader(Shaders::fragmentShaderSource, GL_FRAGMENT_SHADER);
    if (!shaderCorrectlyCompiled(fragmentShaderId)) {
        reportShaderCompileError(fragmentShaderId);
        glfwTerminate();
        return 5;
    }

    // Create Shader programm
    unsigned int shaderProgrammId = glCreateProgram();

    // Attach shaders to shader programm
    glAttachShader(shaderProgrammId, vertexShaderId);
    glAttachShader(shaderProgrammId, fragmentShaderId);

    // Link shader programm
    glLinkProgram(shaderProgrammId);
    if (!programmCorrectlyLinked(shaderProgrammId)) {
        reportProgrammLinkError(shaderProgrammId);
        glfwTerminate();
        return 6;
    }

    // Triangle vertices
    float trVertices[] = {
        -0.5f, -0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f,
        +0.0f, +0.5f, +0.0f
    };

    // Dont need these anymore
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 5. Render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        clearScreen();

        // After this call every render and shader function will use our shaders
        glUseProgram(shaderProgrammId);

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
    glDeleteProgram(shaderProgrammId);

    // Deallocate GLFW resources
    glfwTerminate();
    return 0;
}