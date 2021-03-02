#ifndef OGL_E1_INSTANCESRENDERER_H
#define OGL_E1_INSTANCESRENDERER_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    class InstancesRenderer {
     protected:
        Shader m_shader;

     public:
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

#endif // OGL_E1_INSTANCESRENDERER_H
