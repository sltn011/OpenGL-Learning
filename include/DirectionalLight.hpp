/**
* @file DirectionalLight.hpp
*/

#ifndef OGL_DIRECTIONALLIGHT_H
#define OGL_DIRECTIONALLIGHT_H

#include "BasicLight.hpp"
#include <string>

namespace OGL {

    /**
     * @brief Class that implements DirectionalLight source (sun light)
    */
    class DirectionalLight : public BasicLight {
     public:
        /// Direction of light's rays
        glm::vec3 m_direction;

     public:
        /**
         * @brief DirectionalLight constructor
         * @param direction Light ray direction unit vector
         * @param color Light source RGB color represented as floats 
        */
        DirectionalLight( 
            glm::vec3 direction, 
            glm::vec3 color = glm::vec3{1.0f, 1.0f, 1.0f}
        );

        /**
         * @brief Loads DirectionalLight into Shader's uniform directionalLight value
         * @param shader Currently active Shader - target for loading DirectionalLight into
        */
        virtual void loadInShader( 
            OGL::Shader &shader
        ) override;

        /**
         * @brief Loads DirectionalLight into Shader's uniform directionalLight[indexInArray] value
         * @param shader Currently active Shader - target for loading DirectionalLight into
         * @param indexInArray Index of light in uniform array in Shader
        */
        void loadInShader( 
            OGL::Shader &shader, 
            int indexInArray
        ) override;
    };

} // OGL

#endif // OGL_DIRECTIONALLIGHT_H
