/**
* @file InstancesRenderer.hpp
*/

#ifndef OGL_E1_INSTANCESRENDERER_H
#define OGL_E1_INSTANCESRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    /**
     * @brief Class to render instanced Objects
    */
    class InstancesRenderer {
     protected:
        /// Shader used to render instanced Objects
        Shader m_shader;

     public:
        /**
         * @brief InstancesRenderer constructor
         * @param shader Shader used to render instanced Objectss
        */
        InstancesRenderer(
            Shader &&shader
        );

        InstancesRenderer(
            InstancesRenderer const &rhs
        ) = delete;

        InstancesRenderer(
            InstancesRenderer &&rhs
        ) = default;

        InstancesRenderer &operator=(
            InstancesRenderer const &rhs
        ) = delete;

        InstancesRenderer &operator=(
            InstancesRenderer &&rhs
        ) = default;

        /**
         * @brief Renders Scene's instanced Objects
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

#endif // OGL_E1_INSTANCESRENDERER_H
