#ifndef OGL_E1_NORMALRENDERER_H
#define OGL_E1_NORMALRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    class NormalRenderer {
     public:
        NormalRenderer( 
            Shader &&shader
        );

        NormalRenderer( 
            NormalRenderer const &rhs
        ) = delete;

        NormalRenderer( 
            NormalRenderer &&rhs
        ) = default;

        NormalRenderer &operator=( 
            NormalRenderer const &rhs
        ) = delete;

        NormalRenderer &operator=(
            NormalRenderer &&rhs
        ) = default;

        void render( 
            Scene &scene, 
            BasicCamera const *camera
        );

        void setShader( 
            Shader &&shader
        );

     protected:
        Shader m_shader;
    };

} // OGL::E1

#endif // OGL_E1_NORMALRENDERER_H
