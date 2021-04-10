#ifndef OGL_E1_SKYBOXRENDERER_H
#define OGL_E1_SKYBOXRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    class SkyboxRenderer {
     protected:
        Shader m_shader;

     public:
        SkyboxRenderer(
            Shader &&shader
        );

        SkyboxRenderer( 
            SkyboxRenderer const &rhs
        ) = delete;

        SkyboxRenderer(
            SkyboxRenderer &&rhs
        ) = default;

        SkyboxRenderer &operator=( 
            SkyboxRenderer const &rhs
        ) = delete;

        SkyboxRenderer &operator=( 
            SkyboxRenderer &&rhs
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

#endif // OGL_E1_SKYBOXRENDERER_H
