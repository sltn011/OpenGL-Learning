#ifndef OGL_E1_SHADOWMAPRENDERER_H
#define OGL_E1_SHADOWMAPRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "ShadowMap.hpp"
#include "CameraShadowMap.hpp"

namespace OGL::E1 {

    class ShadowMapRenderer {
     public:
        ShadowMapRenderer(
            Shader &&shader
        );

        ShadowMapRenderer(
            NormalRenderer const &rhs
        ) = delete;

        ShadowMapRenderer(
            ShadowMapRenderer &&rhs
        ) = default;

        ShadowMapRenderer &operator=(
            ShadowMapRenderer const &rhs
        ) = delete;

        ShadowMapRenderer &operator=(
            ShadowMapRenderer &&rhs
        ) = default;

        ShadowMap render(
            Scene &scene,
            CameraShadowMap camera,
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

#endif // OGL_E1_SHADOWMAPRENDERER_H
