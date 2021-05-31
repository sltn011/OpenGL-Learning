/**
* @file TransparentRenderer.hpp
*/

#ifndef OGL_E1_TRANSPARENTRENDERER_H
#define OGL_E1_TRANSPARENTRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include <map>

namespace OGL::E1 {

    /**
     * @brief Class to render transparent Objects
    */
    class TransparentRenderer {
     protected:
        /// Shader used to render transparent Objects
        Shader m_shader;

     public:
        /**
         * @brief TransparentRenderer constructor
         * @param shader Shader used to render transparent Objects
        */
        explicit TransparentRenderer(
            Shader &&shader
        );

        TransparentRenderer( 
            TransparentRenderer const &rhs
        ) = delete;

        TransparentRenderer( 
            TransparentRenderer &&rhs
        ) = default;

        TransparentRenderer &operator=( 
            TransparentRenderer const &rhs
        ) = delete;

        TransparentRenderer &operator=( 
            TransparentRenderer &&rhs
        ) = default;

        /**
         * @brief Renders Scene's transparent Objects
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

#endif // OGL_E1_TRANSPARENTRENDERER_H
