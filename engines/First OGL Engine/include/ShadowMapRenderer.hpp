/**
* @file ShadowMapRenderer.hpp
*/

#ifndef OGL_E1_SHADOWMAPRENDERER_H
#define OGL_E1_SHADOWMAPRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "ShadowMap.hpp"
#include "CameraShadowMap.hpp"

namespace OGL::E1 {

    /**
     * @brief Class to render ShadowMaps
    */
    class ShadowMapRenderer {
     protected:
        /// Shader used to render ShadowMaps
        Shader m_shader;

     public:
        /**
         * @brief ShadowMapRenderer constructor
         * @param shader Shader used to render ShadowMaps
        */
        explicit ShadowMapRenderer(
            Shader &&shader
        );

        ShadowMapRenderer(
            ShadowMapRenderer const &rhs
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

        /**
         * @brief Renders Scene into ShadowMap from position of CameraShadowMap
         * @param scene Scene to be rendered into ShadowMap
         * @param camera Camera used to render ShadowMap
         * @param mapTextureUnit ShadowMap's texture unit GL_TEXTURE{N}
         * @param mapSize Size of ShadowMap's side
         * @return Created ShadowMap
        */
        ShadowMap render(
            Scene &scene,
            CameraShadowMap camera,
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

#endif // OGL_E1_SHADOWMAPRENDERER_H
