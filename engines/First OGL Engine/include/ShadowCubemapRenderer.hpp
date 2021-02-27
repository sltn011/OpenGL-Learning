#ifndef OGL_E1_SHADOWCUBEMAPRENDERER_H
#define OGL_E1_SHADOWCUBEMAPRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "ShadowCubemap.hpp"
#include "CameraShadowCubemap.hpp"

namespace OGL::E1 {

    class ShadowCubemapRenderer {
     public:
        ShadowCubemapRenderer(
            Shader &&shader
        );

        ShadowCubemapRenderer(
            NormalRenderer const &rhs
        ) = delete;

        ShadowCubemapRenderer(
            ShadowCubemapRenderer &&rhs
        ) = default;

        ShadowCubemapRenderer &operator=(
            ShadowCubemapRenderer const &rhs
        ) = delete;

        ShadowCubemapRenderer &operator=(
            ShadowCubemapRenderer &&rhs
            ) = default;

        ShadowCubemap render(
            Scene &scene,
            CameraShadowCubemap camera,
            GLenum mapTextureUnit,
            int mapSize
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

#endif // OGL_E1_SHADOWCUBEMAPRENDERER_H
