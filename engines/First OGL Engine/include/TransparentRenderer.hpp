#ifndef OGL_E1_TRANSPARENTRENDERER_H
#define OGL_E1_TRANSPARENTRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include <map>

namespace OGL::E1 {

    class TransparentRenderer {
     protected:
        Shader m_shader;

     public:
        TransparentRenderer( 
            Shader &&shader
        );

        TransparentRenderer( 
            TransparentRenderer const &rhs
        ) = delete;

        TransparentRenderer( 
            TransparentRenderer &&rhs
        ) = default;

        TransparentRenderer &operator=( 
            TransparentRenderer const &rhs
        ) = delete;

        TransparentRenderer &operator=( 
            TransparentRenderer &&rhs
        ) = default;

        void render(
            Scene &scene, 
            BasicCamera const *camera
        );

        void setShader(
            Shader &&shader
        );

        Shader &getShader(
        );
    };

} // OGL::E1

#endif // OGL_E1_TRANSPARENTRENDERER_H
