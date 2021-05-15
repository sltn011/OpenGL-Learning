/**
* @file SkyboxRenderer.hpp
*/

#ifndef OGL_E1_SKYBOXRENDERER_H
#define OGL_E1_SKYBOXRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    /**
     * @brief Class to render Skybox
    */
    class SkyboxRenderer {
     protected:
        /// Shader used to render Skybox
        Shader m_shader;

     public:
        /**
         * @brief SkyboxRenderer constructor
         * @param shader Shader used to render Skybox
        */
        explicit SkyboxRenderer(
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

        /**
         * @brief Render Scene's Skybox
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

#endif // OGL_E1_SKYBOXRENDERER_H
