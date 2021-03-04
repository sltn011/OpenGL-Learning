#ifndef OGL_CAMERASHADOWMAP_H
#define OGL_CAMERASHADOWMAP_H

#include "BasicCamera.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"

namespace OGL {

    class CameraShadowMap : public BasicCamera {
        float m_scale;
        bool  m_hasPerspective;

     public:
        CameraShadowMap(
            DirectionalLight dirLight,
            glm::vec3 sceneCenter,
            float sceneDownscale,
            float nearPlane,
            float farPlane
        );

        CameraShadowMap(
            SpotLight spotLight,
            float nearPlane,
            float farPlane
        );

        glm::mat4 getViewMatrix(
        ) const override;

        glm::mat4 getProjectionMatrix(
        ) const override;

        void processMoveInput(
            CameraMovementDirection dir,
            float deltaTime
        ) override;

        void processRotateInput(
            float xOffset,
            float yOffset,
            float sensitivity,
            bool constrainPitch
        ) override;
    };

}

#endif // OGL_CAMERASHADOWMAP_H
