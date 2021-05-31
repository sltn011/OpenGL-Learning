/**
* @file CameraShadowMap.hpp
*/

#ifndef OGL_CAMERASHADOWMAP_H
#define OGL_CAMERASHADOWMAP_H

#include "BasicCamera.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"

namespace OGL {

    /**
     * @brief Camera class to render shadow map produced by DirectionalLight or SpotLight
    */
    class CameraShadowMap : public BasicCamera {
        /// Coefficient used to scale viewed scene when using orthographic projection matrix
        float m_scale;

        /// Set to false if perspective division should NOT be performed(when creating shadows from DirectionalLight)
        bool  m_hasPerspective;

     public:
        /**
         * @brief CameraShadowMap constructor for rendering shadow map from DirectionalLight(no perspective division)
         * @param dirLight DirectionalLight for which shadow map will be rendered
         * @param sceneCenter Center position in worldspace of area being lit by light
         * @param sceneDownscale Multiplier that reduces scene size while rendering
         * @param near Near clip plane
         * @param far Far clip plane
        */
        CameraShadowMap(
            DirectionalLight dirLight,
            glm::vec3 sceneCenter,
            float sceneDownscale,
            float near,
            float far
        );

        /**
         * @brief CameraShadowMap constructor for rendering shadow map from SpotLight(with perspective division)
         * @param spotLight SpotLight for which shadow map will be rendered
         * @param near Near clip plane
         * @param far Far clip plane
        */
        CameraShadowMap(
            SpotLight spotLight,
            float near,
            float far
        );

        /**
         * @brief Get method of camera's view matrix
         * @return 4x4 view matrix
        */
        glm::mat4 getViewMatrix(
        ) const override;

        /**
         * @brief Get method of camera's projection matrix
         * @return 4x4 projection matrix (no perspective division for DirectionalLight and with division for SpotLight)
        */
        glm::mat4 getProjectionMatrix(
        ) const override;

        /**
         * @brief Override of BasicCamera move process method - does nothing
         * @param dir unused value
         * @param deltaTime unused value
        */
        void processMoveInput(
            CameraMovementDirection dir,
            float deltaTime
        ) override;

        /**
         * @brief Override of BasicCamera rotation process method - does nothing
         * @param xOffset unused value
         * @param yOffset unused value
         * @param sensitivity unused value
         * @param constrainPitch unused value
        */
        void processRotateInput(
            float xOffset,
            float yOffset,
            float sensitivity,
            bool constrainPitch
        ) override;
    };

}

#endif // OGL_CAMERASHADOWMAP_H
