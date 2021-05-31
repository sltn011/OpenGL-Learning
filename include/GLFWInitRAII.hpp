/**
* @file GLFWInitRAII.hpp
*/

#ifndef OGL_GLFWINITRAII_H
#define OGL_GLFWINITRAII_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Exception.hpp"

namespace OGL {

    /**
     * @brief Class that provides initialization of GLFW context following RAII idiom
    */
    class GLFWInitRAII {

        // Count of instances of this class
        static int s_numberOfInstances;

     public:

        /**
         * @brief Constructor that initializes GLFW
        */
        GLFWInitRAII(
        );

        /**
         * @brief Destructor that deallocates GLFW data
        */
        ~GLFWInitRAII(
        );

        GLFWInitRAII( 
            GLFWInitRAII const &rhs
        );

        GLFWInitRAII &operator=( 
            GLFWInitRAII const &rhs
        ) = default;

        GLFWInitRAII( 
            GLFWInitRAII &&rhs
        ) noexcept;

        GLFWInitRAII &operator=(
            GLFWInitRAII &&rhs
        ) = default;
    };

} // OGL


#endif // OGL_GLFWINITRAII_H