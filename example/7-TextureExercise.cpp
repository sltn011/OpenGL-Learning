#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texture1;
GLuint texture2;

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
, float *recVertices
, OGL::Shader &shaderProgramm
) {
    static float mixing = 0.5f;
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        recVertices[6] -= 0.01f;
        recVertices[7] -= 0.01f;
        recVertices[14] -= 0.01f;
        recVertices[15] += 0.01f;
        recVertices[22] += 0.01f;
        recVertices[23] += 0.01f;
        recVertices[30] += 0.01f;
        recVertices[31] -= 0.01f;
        glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(recVertices), recVertices, GL_STATIC_DRAW);
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        recVertices[6] += 0.01f;
        recVertices[7] += 0.01f;
        recVertices[14] += 0.01f;
        recVertices[15] -= 0.01f;
        recVertices[22] -= 0.01f;
        recVertices[23] -= 0.01f;
        recVertices[30] -= 0.01f;
        recVertices[31] += 0.01f;
        glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(recVertices), recVertices, GL_STATIC_DRAW);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shaderProgramm.use();
        mixing += 0.01f;
        if (mixing >= 1.0f) {
            mixing = 1.0f;
        }
        shaderProgramm.setUniformFloat("mixing", mixing);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shaderProgramm.use();
        mixing -= 0.01f;
        if (mixing <= 0.0f) {
            mixing = 0.0f;
        }
        shaderProgramm.setUniformFloat("mixing", mixing);
    }
}

void switchTexFilter
( GLFWwindow *window
, int key
, int scancode
, int action
, int mode
) {
    static int texModeFilterMin[] = {GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR};
    static int TMFMinI = 0;

    static int texModeFilterMag[] = {GL_NEAREST, GL_LINEAR};
    static int TMFMagI = 0;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texModeFilterMin[(++TMFMinI) % 6]);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texModeFilterMin[TMFMinI]);
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texModeFilterMin[(++TMFMagI) % 2]);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texModeFilterMin[TMFMagI]);
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
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Hello, Texture", nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 2;
    }

    // Set window as current context
    glfwMakeContextCurrent(window);

    // Set callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Key callback
    glfwSetKeyCallback(window, switchTexFilter);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    OGL::Shader shaderProgramm("shaders/3-texExercise.vert", "shaders/3-texExercise.frag");

    // Rectangle vertices
    float recVertices[] = {
        // Position             // Color            // Texture
        -0.5f, -0.5f, +0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, +0.5f, +0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        +0.5f, +0.5f, +0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        +0.5f, -0.5f, +0.0f,    1.0f, 1.0f, 0.0f,   1.0f, 0.0f
    };

    // Rectangle indices
    unsigned int recIndices[] = {
        0, 1, 2,
        0, 2, 3
    };


    // Textures
    // Creating texture id
    glGenTextures(1, &texture1);
    
    // Load texture image using stb library
    int width, height, nrChannels;

    unsigned char *data;
    data = stbi_load("textures/bricks.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        throw OGL::Exception("Error loading texture!");
    }

    // Bind texture so next texture calls use this object
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Wrap around - S = X, T = Y, Z = R
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (!data) {
        throw OGL::Exception("Error loading texture!");
    }

    glGenTextures(1, &texture2);

    // Bind texture so next texture calls use this object
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Wrap around - S = X, T = Y, Z = R
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);


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
    glBufferData(GL_ARRAY_BUFFER, sizeof(recVertices), recVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(recIndices), recIndices, GL_STATIC_DRAW);

    // 5. Tell OpenGL how to interpret vertices data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Draw wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // to undo - glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); - to default

    shaderProgramm.use(); // MUST!!!
    shaderProgramm.setUniformInt("myTexture1", 0);
    shaderProgramm.setUniformInt("myTexture2", 1);
    shaderProgramm.setUniformFloat("mixing", 0.5f);

    // 6. Render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window, recVertices, shaderProgramm);

        clearScreen();

        // After this call every render and shader function will use our shaders
        shaderProgramm.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);       
        
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