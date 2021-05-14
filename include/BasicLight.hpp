/**
* @file BasicLight.hpp
*/

#ifndef OGL_BASICLIGHT_H
#define OGL_BASICLIGHT_H

#include "glm/glm.hpp"

#include "Shader.hpp"

namespace OGL {

    /**
     * @brief Basic class to implement light sources
    */
    class BasicLight {
     public:
        /// RGB Color of emitted light represented as floats 
        glm::vec3 m_color;

     public:

        /**
         * @brief Basic light constructor
         * @param color RGB Color of emitted light represented as floats 
        */
        BasicLight(
            glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f }
        );

        /**
         * @brief Virtual basic light destructor
        */
        virtual ~BasicLight();

        /**
         * @brief Abstract method to load single light into Shader's uniform light structure
         * @param shader Currently active Shader - target for loading light into
        */
        virtual void loadInShader(
            OGL::Shader &shader
        ) = 0;

        /**
         * @brief Abstract method to load light into Shader's uniform array of light structures
         * @param shader Currently active Shader - target for loading light into
         * @param indexInArray Index of light in uniform array in Shader
        */
        virtual void loadInShader(
            OGL::Shader &shader, 
            int indexInArray
        ) = 0;

    };

} // OGL

#endif // OGL_BASICLIGHT_H
