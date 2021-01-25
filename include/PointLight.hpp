#ifndef OGL_POINTLIGHT_H
#define OGL_POINTLIGHT_H

#include "BasicLight.hpp"
#include <string>

namespace OGL {

    class PointLight : public BasicLight {
     public:
        glm::vec3 m_position;
        float     m_attenuationConst;
        float     m_attenuationLinear;
        float     m_attenuationQuadratic;


     public:
        PointLight( 
            glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}, 
            glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f }, 
            float attenuationConst = 1.0f, 
            float attenuationLinear = 0.09f, 
            float attenuationQuadratic = 0.032f
        );

        virtual void loadInShader( 
            OGL::Shader &shader
        ) override;

        void loadInShader( 
            OGL::Shader &shader, 
            int indexInArray
        ) override;
    };

} // OGL

#endif // OGL_POINTLIGHT_H
