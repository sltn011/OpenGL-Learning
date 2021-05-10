/**
* @file PointLight.hpp
*/

#ifndef OGL_POINTLIGHT_H
#define OGL_POINTLIGHT_H

#include "BasicLight.hpp"
#include <string>

namespace OGL {

    /**
     * @brief Class that implements PointLight source
    */
    class PointLight : public BasicLight {
     public:
        /// PointLight position in world space
        glm::vec3 m_position;

        /// Attenuation constant term
        float     m_attenuationConst;

        /// Attenuation linear term
        float     m_attenuationLinear;

        /// Attenuation quadratic term
        float     m_attenuationQuadratic;


     public:
        /**
         * @brief PointLight constructor
         * @param position PointLight position in world space
         * @param color PointLight color
         * @param attenuationConst Attenuation constant term
         * @param attenuationLinear Attenuation linear term
         * @param attenuationQuadratic Attenuation quadratic term
        */
        PointLight( 
            glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}, 
            glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f }, 
            float attenuationConst = 1.0f, 
            float attenuationLinear = 0.09f, 
            float attenuationQuadratic = 0.032f
        );

        /**
         * @brief Loads PointLight into Shader's uniform pointLight value
         * @param shader Currently active Shader - target for loading PointLight into
        */
        virtual void loadInShader( 
            OGL::Shader &shader
        ) override;

        /**
         * @brief Loads PointLight into Shader's uniform pointLight[indexInArray] value
         * @param shader Currently active Shader - target for loading PointLight into
         * @param indexInArray Index of light in uniform array in Shader
        */
        void loadInShader( 
            OGL::Shader &shader, 
            int indexInArray
        ) override;
    };

} // OGL

#endif // OGL_POINTLIGHT_H
