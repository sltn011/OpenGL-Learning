#ifndef OGL_GLFWINITRAII_H
#define OGL_GLFWINITRAII_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Exception.hpp"

namespace OGL {

    struct GLFWInitRAII {
        GLFWInitRAII
        (
        );

        ~GLFWInitRAII
        (
        );

        GLFWInitRAII
        ( GLFWInitRAII const &rhs
        ) = delete;

        GLFWInitRAII &operator=
        ( GLFWInitRAII const &rhs
        ) = delete;

        GLFWInitRAII
        ( GLFWInitRAII &&rhs
        ) = default;
    };

} // OGL


#endif // OGL_GLFWINITRAII_H