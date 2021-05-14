/**
* @file LightSourcesDebugRenderer.hpp
*/

#ifndef OGL_E1_LIGHTSOURCESDEBUGRENDERER_H
#define OGL_E1_LIGHTSOURCESDEBUGRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

namespace OGL::E1 {

    /**
     * @brief Class that renders Scene's PointLights and SpotLights as colored cubes
    */
    class LightSourcesDebugRenderer {
     public:
         /// Cube's VAO
         VertexArrayObject   m_vao;

         /// Cube's VBO
         VertexBufferObject  m_vbo;

         /// Cube's EBO
         ElementBufferObject m_ebo;

         /// Cube's scale
         float               m_lightCubeSize;

     protected:
        /// Shader used to render colored cubes
        Shader m_shader;

     public:
        /**
         * @brief LightSourcesDebugRenderer constructor
         * @param shader Shader used to render colored cubes
         * @param lightCubeSize Cubes' scale
        */
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

        /**
         * @brief Renders Scene's PointLights and SpotLights as colored cubes
         * @param scene Scene used in rendering
         * @param camera Camera used in rendering
        */
        void render(
            Scene &scene,
            BasicCamera const *camera
        );

        /**
         * @brief Replace Shader with new one
         * @param shader New Shader object
        */
        void setShader(
            Shader &&shader
        );

        /**
         * @brief Get reference to owned Shader object
         * @return Reference to owned Shader object
        */
        Shader &getShader(
        );
    };

} // OGL::E1

#endif // OGL_E1_LIGHTSOURCESDEBUGRENDERER_H
