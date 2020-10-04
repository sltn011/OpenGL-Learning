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

    char const *fragmentShaderSourceOrange =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}"
    ;

    char const *fragmentShaderSourcePurple =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
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
    unsigned int fragmentShaderIdOrange = compileGLShader(Shaders::fragmentShaderSourceOrange, GL_FRAGMENT_SHADER);
    if (!shaderCorrectlyCompiled(fragmentShaderIdOrange)) {
        reportShaderCompileError(fragmentShaderIdOrange);
        glfwTerminate();
        return 5;
    }

    // Compile Fragment Shader
    unsigned int fragmentShaderIdPurple = compileGLShader(Shaders::fragmentShaderSourcePurple, GL_FRAGMENT_SHADER);
    if (!shaderCorrectlyCompiled(fragmentShaderIdPurple)) {
        reportShaderCompileError(fragmentShaderIdPurple);
        glfwTerminate();
        return 5;
    }

    // Create Shader programm
    unsigned int shaderProgrammId1 = glCreateProgram();
    unsigned int shaderProgrammId2 = glCreateProgram();

    // Attach shaders to shader programm
    glAttachShader(shaderProgrammId1, vertexShaderId);
    glAttachShader(shaderProgrammId2, vertexShaderId);

    glAttachShader(shaderProgrammId1, fragmentShaderIdOrange);
    glAttachShader(shaderProgrammId2, fragmentShaderIdPurple);

    // Link shader programm
    glLinkProgram(shaderProgrammId1);
    if (!programmCorrectlyLinked(shaderProgrammId1)) {
        reportProgrammLinkError(shaderProgrammId1);
        glfwTerminate();
        return 6;
    }

    glLinkProgram(shaderProgrammId2);
    if (!programmCorrectlyLinked(shaderProgrammId2)) {
        reportProgrammLinkError(shaderProgrammId2);
        glfwTerminate();
        return 6;
    }

    // Triangle vertices
    float trVertices[] = {
        -0.5f, -0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f,
        +0.0f, +0.5f, +0.0f,

        -0.9f, +0.9f, +0.0f,
        -0.3f, +0.8f, +0.0f,
        -0.9f, +0.6f, +0.0f
    };

    // Dont need these anymore
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderIdOrange);
    glDeleteShader(fragmentShaderIdPurple);

    // Initialization code - done once(unless objects change a lot)
    // 1. Create
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    // 2. Creating vertex buffer - stores A LOT of vertices in GPU memory for processing
    unsigned int VBO[2]; // Vertices Buffer Object id
    glGenBuffers(2, VBO);

    // First triangle
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(trVertices[0]), trVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Second triangle
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(trVertices[9]), &trVertices[9], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 5. Render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        clearScreen();

        glBindVertexArray(VAO[0]);
        glUseProgram(shaderProgrammId1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO[1]);
        glUseProgram(shaderProgrammId2);
        glDrawArrays(GL_TRIANGLES, 0, 3);      
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();
    }

    // Free resources
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgrammId1);
    glDeleteProgram(shaderProgrammId2);

    // Deallocate GLFW resources
    glfwTerminate();
    return 0;
}