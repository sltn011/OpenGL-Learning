#ifndef OGL_E1_LIGHTSOURCESDEBUGRENDERER_H
#define OGL_E1_LIGHTSOURCESDEBUGRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

namespace OGL::E1 {

    class LightSourcesDebugRenderer {
     public:
         VertexArrayObject   m_vao;
         VertexBufferObject  m_vbo;
         ElementBufferObject m_ebo;
         float               m_lightCubeSize;

     protected:
        Shader m_shader;

     public:
        LightSourcesDebugRenderer(
            Shader &&shader,
            float lightCubeSize
        );

        LightSourcesDebugRenderer(
            LightSourcesDebugRenderer const &rhs
        ) = delete;

        LightSourcesDebugRenderer(
            LightSourcesDebugRenderer &&rhs
        ) = default;

        LightSourcesDebugRenderer &operator=(
            LightSourcesDebugRenderer const &rhs
        ) = delete;

        LightSourcesDebugRenderer &operator=(
            LightSourcesDebugRenderer &&rhs
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

#endif // OGL_E1_LIGHTSOURCESDEBUGRENDERER_H
