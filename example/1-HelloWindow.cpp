#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

enum Screen {
    width = 1000,
    height = 800
};

void framebufferSizeCallback( 
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
}

void render(
    GLFWwindow *window
) {
    glClearColor(0.3f, 0.15f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(
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
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Hello, Window", NULL, NULL);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Set window as current context
    glfwMakeContextCurrent(window);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    // Set dimensions of rendering window
    glViewport(0, 0, Screen::width, Screen::height);

    // Set callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        render(window);
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();
    }

    // Deallocate GLFW resources
    glfwTerminate();
    return 0;
}