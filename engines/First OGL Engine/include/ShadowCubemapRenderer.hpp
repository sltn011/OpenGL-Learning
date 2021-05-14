/**
* @file ShadowCubemapRenderer.hpp
*/

#ifndef OGL_E1_SHADOWCUBEMAPRENDERER_H
#define OGL_E1_SHADOWCUBEMAPRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "ShadowCubemap.hpp"
#include "CameraShadowCubemap.hpp"

namespace OGL::E1 {

    /**
     * @brief Class to render ShadowCubemaps
    */
    class ShadowCubemapRenderer {
     protected:
        /// Shader used to render ShadowCubemaps
        Shader m_shader;

     public:
        /**
         * @brief ShadowCubemapRenderer constructor
         * @param shader Shader used to render ShadowCubemaps
        */
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

        /**
         * @brief Renders Scene into ShadowCubemap from position of CameraShadowCubemap
         * @param scene Scene to be rendered into ShadowCubemap
         * @param camera Camera used to render ShadowCubemap
         * @param mapTextureUnit ShadowCubemap's texture unit GL_TEXTURE{N}
         * @param mapSize Size of ShadowCubemap's side
         * @return Created ShadowCubemap
        */
        ShadowCubemap render(
            Scene &scene,
            CameraShadowCubemap camera,
            GLenum mapTextureUnit,
            int mapSize
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

#endif // OGL_E1_SHADOWCUBEMAPRENDERER_H
