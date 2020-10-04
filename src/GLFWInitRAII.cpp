#include "GLFWInitRAII.hpp"

namespace OGL {

    GLFWInitRAII::GLFWInitRAII
    (
    ) {
        if (glfwInit() == GL_FALSE) {
            throw Exception("Error initializing GLFW!");
        }
    }

    GLFWInitRAII::~GLFWInitRAII
    (
    ) {
        glfwTerminate();
    }

} // OGL