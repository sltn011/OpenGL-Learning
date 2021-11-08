/**
* @file Shader.hpp
*/

#ifndef OGL_SHADER_H
#define OGL_SHADER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include "Exception.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace OGL {

    /**
     * @brief Class for compiling and working with Shader programm
    */
    class Shader {
        /// Shader programm ID
        unsigned int m_programmID;

        /// If True will output Shader errors to std::cerr
        bool         m_showWarnings;

        /**
         * @brief Compiles shader object
         * @param sourceCode Pointer to null-terminated string with shader object source code
         * @param shaderType Type of shader object
         * @return shader object ID
        */
        unsigned int compileGLShader( 
            char const *sourceCode, 
            int shaderType
        );

        /**
         * @brief Check if shader was successfully compiled
         * @param shaderId ID of shader object
         * @return True if compiled shader object compiled correctly, False otherwise
        */
        bool shaderCorrectlyCompiled( 
            unsigned int shaderId
        );

        /**
         * @brief Throws Exception with shader object compile error message
         * @param shaderId ID of shader object
        */
        [[noreturn]] void reportShaderCompileError( 
            unsigned int shaderId
        );

        /**
         * @brief Check if Shader programm was successfully linked
         * @param programmId ID of Shader programm
         * @return True if Shader programm linked correctly, False otherwise
        */
        bool programmCorrectlyLinked( 
            unsigned int programmId
        );

        /**
         * @brief Throws Exception with Shader programm link error message
         * @param programmId ID of Shader programm
        */
        [[noreturn]] void reportProgrammLinkError( 
            unsigned int programmId
        );

        /**
         * @brief Prints invalid uniform location message to std::cerr
         * @param name Name of invalid uniform variable
        */
        void warnInvalidUniformLocation( 
            std::string const &name
        );

     public:
        /**
         * @brief Creates Shader programm with vertex and fragment shaders
         * @param vertexSourcePath Path to file with vertex shader code
         * @param fragmentSourcePath Path to file with fragment shader code
        */
        Shader( 
            std::string vertexSourcePath,
            std::string fragmentSourcePath
        );

        /**
         * @brief Creates Shader programm with vertex, geometry and fragment shaders
         * @param vertexSourcePath Path to file with vertex shader code
         * @param geometrySourcePath Path to file with geometry shader code
         * @param fragmentSourcePath Path to file with fragment shader code
        */
        Shader( 
            std::string vertexSourcePath, 
            std::string geometrySourcePath,
            std::string fragmentSourcePath
        );

        Shader( 
            Shader const &rhs
        ) = delete;

        /**
         * @brief Shader move constructor
         * @param rhs Moved value
        */
        Shader( 
            Shader &&rhs
        ) noexcept;

        Shader &operator=(
            Shader const &rhs
        ) = delete;

        /**
         * @brief Shader move assignment
         * @param rhs Moved value
         * @return Reference to *this
        */
        Shader &operator=( 
            Shader &&rhs
        ) noexcept;

        /**
         * @brief Shader destructor
        */
        ~Shader(
        );

        /**
         * @brief Get Shader programm ID
         * @return Shader programm ID
        */
        unsigned int id(
        ) const;

        /**
         * @brief Sets this Shader programm as active
        */
        void use (
        );

        /**
         * @brief Select if Shader errors should be reported to std::cerr
         * @param value True if Shader errors should be reported to std::cerr
        */
        void showWarnings( 
            bool value
        );

        /**
         * @brief Sets value to "uniform bool name" if Shader is active
         * @param name Name of uniform bool variable
         * @param val Boolean value to be assigned to uniform variable
         * @return True if successful, False otherwise
        */
        bool setUniformBool( 
            std::string const &name,
            bool val
        );

        /**
         * @brief Sets value to "uniform int name" if Shader is active
         * @param name Name of uniform int variable
         * @param val Int value to be assigned to uniform variable
         * @return True if successful, False otherwise
        */
        bool setUniformInt( 
            std::string const &name,
            int val
        );

        /**
         * @brief Sets value to "uniform float name" if Shader is active
         * @param name Name of uniform float variable
         * @param val Float value to be assigned to uniform variable
         * @return True if successful, False otherwise
        */
        bool setUniformFloat(
            std::string const &name, 
            float val
        );

        /**
         * @brief Sets value to "uniform mat4 name" if Shader is active
         * @param name Name of uniform mat4 variable
         * @param val Mat4 value to be assigned to uniform variable
         * @param doTranspose True if matrix should be transposed
         * @return True if successful, False otherwise
        */
        bool setUniformMatrix4( 
            std::string const &name,
            glm::mat4 const &val,
            bool doTranspose = false
        );

        /**
         * @brief Sets value to "uniform vec3 name" if Shader is active
         * @param name Name of uniform vec3 variable
         * @param val Vec3 value to be assigned to uniform variable
         * @return True if successful, False otherwise
        */
        bool setUniformVec3( 
            std::string const &name, 
            glm::vec3 const &val
        );

        /**
         * @brief Sets value to "uniform vec4 name" if Shader is active
         * @param name Name of uniform vec4 variable
         * @param val Vec4 value to be assigned to uniform variable
         * @return True if successful, False otherwise
        */
        bool setUniformVec4(
            std::string const &name,
            glm::vec4 const &val
        );

        /**
         * @brief Assigns binding point to an active uniform block
         * @param blockName Name of uniform block
         * @param bindingPointIndex Index of a binding point
         * @return 
        */
        bool uniformBlockBinding( 
            std::string const &blockName, 
            size_t bindingPointIndex
        );
    };

} // OGL

#endif // OGL_SHADER_H