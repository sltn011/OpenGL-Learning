/**
* @file NormalRenderer.hpp
*/

#ifndef OGL_E1_NORMALRENDERER_H
#define OGL_E1_NORMALRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    /**
     * @brief Class to render normal Objects
    */
    class NormalRenderer {
     protected:
        /// Shader used to render normal Objects
        Shader m_shader;

     public:
        /**
         * @brief NormalRenderer constructor
         * @param shader Shader used to render normal Objects
        */
        explicit NormalRenderer(
            Shader &&shader
        );

        NormalRenderer( 
            NormalRenderer const &rhs
        ) = delete;

        NormalRenderer( 
            NormalRenderer &&rhs
        ) = default;

        NormalRenderer &operator=( 
            NormalRenderer const &rhs
        ) = delete;

        NormalRenderer &operator=(
            NormalRenderer &&rhs
        ) = default;

        /**
         * @brief Renders Scene's normal Objects
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

#endif // OGL_E1_NORMALRENDERER_H
