/**
* @file SpotLight.hpp
*/

#ifndef OGL_SPOTLIGHT_H
#define OGL_SPOTLIGHT_H

#include "PointLight.hpp"
#include <string>

namespace OGL {

    /**
     * @brief Class that implements SpotLight source
    */
    class SpotLight : public PointLight {
     public:
        /// Direction of light's rays
        glm::vec3 m_direction;

        /// Inner cutoff angle in degrees
        float     m_cutOffAngle;

        /// Outer cutoff angle in degrees
        float     m_cutOffOuterAngle;

     public:
        /**
         * @brief SpotLight constructor
         * @param position SpotLight position in world space
         * @param direction Direction of light's rays
         * @param color SpotLight RGB color represented as floats 
         * @param cutOffAngle Inner cutoff angle in degrees
         * @param cutOffOuterAngle Outer cutoff angle in degrees
         * @param attenuationConst Attenuation constant term
         * @param attenuationLinear Attenuation linear term
         * @param attenuationQuadratic Attenuation quadratic term
        */
        SpotLight( 
            glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3 direction = glm::vec3{ 0.0f, 1.0f, 1.0f }, 
            glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f }, 
            float cutOffAngle = 8.0f, 
            float cutOffOuterAngle = 12.0f, 
            float attenuationConst = 1.0f, 
            float attenuationLinear = 0.09f, 
            float attenuationQuadratic = 0.032f
        );

        /**
         * @brief Loads SpotLight into Shader's uniform spotLight value
         * @param shader Currently active Shader - target for loading SpotLight into
        */
        virtual void loadInShader( 
            OGL::Shader &shader
        ) override;

        /**
         * @brief Loads SpotLight into Shader's uniform spotLight[indexInArray] value
         * @param shader Currently active Shader - target for loading SpotLight into
         * @param indexInArray Index of light in uniform array in Shader
        */
        void loadInShader(
            OGL::Shader &shader, 
            int indexInArray
        ) override;
    };

} // OGL

#endif // OGL_SPOTLIGHT_H
