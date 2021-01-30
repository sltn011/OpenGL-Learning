#ifndef OGL_E1_MIRRORRENDERER_H
#define OGL_E1_MIRRORRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    class MirrorRenderer {
     public:
        MirrorRenderer( 
            Shader &&shader
        );

        MirrorRenderer(
            MirrorRenderer const &rhs
        ) = delete;

        MirrorRenderer( 
            MirrorRenderer &&rhs
        ) = default;

        MirrorRenderer &operator=(
            MirrorRenderer const &rhs
        ) = delete;

        MirrorRenderer &operator=(
            MirrorRenderer &&rhs
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

     protected:
        Shader m_shader;
    };

} // OGL::E1

#endif // OGL_E1_MIRRORRENDERER_H
