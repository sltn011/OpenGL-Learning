#ifndef OGL_E1_SKYBOXRENDERER_H
#define OGL_E1_SKYBOXRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    class SkyboxRenderer {
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

     protected:
        Shader m_shader;
    };

} // OGL::E1

#endif // OGL_E1_SKYBOXRENDERER_H
