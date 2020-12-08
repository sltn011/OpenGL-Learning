#ifndef OGL_SPOTLIGHT_H
#define OGL_SPOTLIGHT_H

#include "PointLight.hpp"
#include <string>

namespace OGL {

    class SpotLight : public PointLight {
    public:
        glm::vec3 m_direction;
        float     m_cutOffAngle;
        float     m_cutOffOuterAngle;

    public:
        SpotLight
        ( glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f }
        , glm::vec3 direction = glm::vec3{ 0.0f, 1.0f, 1.0f }
        , glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f }
        , float cutOffAngleRadians = glm::radians(8.0f)
        , float cutOffOuterAngleRadians = glm::radians(12.0f)
        , float attenuationConst = 1.0f
        , float attenuationLinear = 0.09f
        , float attenuationQuadratic = 0.032f
        );

        virtual void loadInShader
        ( OGL::Shader &shader
        ) override;

        void loadInShader
        ( OGL::Shader &shader
        , int indexInArray
        ) override;
    };

} // OGL

#endif // OGL_SPOTLIGHT_H
