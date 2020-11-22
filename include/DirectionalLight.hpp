#ifndef OGL_DIRECTIONALLIGHT_H
#define OGL_DIRECTIONALLIGHT_H

#include "BasicLight.hpp"
#include <string>

namespace OGL {

    class DirectionalLight : public BasicLight {
    public:
        glm::vec3 m_direction;

    public:
        DirectionalLight
        ( glm::vec3 direction
        , glm::vec3 color = glm::vec3{1.0f, 1.0f, 1.0f}
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

#endif // OGL_DIRECTIONALLIGHT_H
