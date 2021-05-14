/**
* @file MirrorRenderer.hpp
*/

#ifndef OGL_E1_MIRRORRENDERER_H
#define OGL_E1_MIRRORRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    /**
     * @brief Class used to render mirror Objects
    */
    class MirrorRenderer {
     protected:
        /// Shader used to render mirror Objects
        Shader m_shader;

     public:
        /**
         * @brief MirrorRenderer constructor
         * @param shader Shader used to render mirror Objects
        */
        MirrorRenderer( 
            Shader &&shader
        );

        MirrorRenderer(
            MirrorRenderer const &rhs
        ) = delete;

        MirrorRenderer( 
            MirrorRenderer &&rhs
        ) = default;

        MirrorRenderer &operator=(
            MirrorRenderer const &rhs
        ) = delete;

        MirrorRenderer &operator=(
            MirrorRenderer &&rhs
        ) = default;

        /**
         * @brief Renders Scene's mirror Objects. Mirror's Cubemap should be rendered or Object will be skipped
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

#endif // OGL_E1_MIRRORRENDERER_H
