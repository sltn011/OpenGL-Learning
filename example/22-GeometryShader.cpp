#include "GLFWInitRAII.hpp"
#include "Shader.hpp"

#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"

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

void clearScreen(
) {
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(
) {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Geometry Shader", nullptr, nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OGL::Exception("Failed to initialize GLAD");
    }

    float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f 
    };

    OGL::VertexArrayObject vao;
    vao.bind();
    OGL::VertexBufferObject vbo;
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    vao.unbind();

    OGL::Shader shader("shaders/18-geomShader.vert", "shaders/18-geomShader.geom", "shaders/18-geomShader.frag");
    shader.use();

    vao.bind();

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        clearScreen();

        glDrawArrays(GL_POINTS, 0, 4);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    return 0;
}