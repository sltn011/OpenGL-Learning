#ifndef OGL_BASICLIGHT_H
#define OGL_BASICLIGHT_H

#include "glm/glm.hpp"

#include "Shader.hpp"

namespace OGL {

    class BasicLight {
    public:
        glm::vec3 m_color;

    public:
        BasicLight
        ( glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f }
        );

        virtual ~BasicLight
        (
        );

        virtual void loadInShader
        ( OGL::Shader &shader
        ) = 0;

        virtual void loadInShader
        ( OGL::Shader &shader
        , int indexInArray
        ) = 0;

    };

} // OGL

#endif // OGL_BASICLIGHT_H
