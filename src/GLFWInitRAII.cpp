#include "GLFWInitRAII.hpp"

namespace OGL {

    int GLFWInitRAII::s_numberOfInstances = 0;

    GLFWInitRAII::GLFWInitRAII(
    ) {
        if (!s_numberOfInstances) {
            if (glfwInit() == GL_FALSE) {
                throw Exception("Error initializing GLFW!");
            }
        }
        ++s_numberOfInstances;
    }

    GLFWInitRAII::~GLFWInitRAII(
    ) {
        --s_numberOfInstances;
        if (!s_numberOfInstances) {
            glfwTerminate();
        }
    }

    GLFWInitRAII::GLFWInitRAII(
        GLFWInitRAII const &rhs
    ) {
        ++s_numberOfInstances;
    }

    GLFWInitRAII::GLFWInitRAII(
        GLFWInitRAII &&rhs
    ) noexcept {
        ++s_numberOfInstances;
    }

} // OGL