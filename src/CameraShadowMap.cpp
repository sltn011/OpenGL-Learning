#include "CameraShadowMap.hpp"

namespace OGL {

    CameraShadowMap::CameraShadowMap(
        DirectionalLight dirLight,
        glm::vec3 sceneCenter,
        float sceneDownscale,
        float nearPlane,
        float farPlane
    ) : BasicCamera {
        sceneCenter - dirLight.m_direction,
        sceneCenter,
        0.0f,
        0.0f,
        1.0f,
        nearPlane,
        farPlane },
        m_scale {sceneDownscale},
        m_hasPerspective{ false } {
    }

    CameraShadowMap::CameraShadowMap(
        SpotLight spotLight,
        float nearPlane,
        float farPlane
    ) : BasicCamera {
        spotLight.m_position,
        spotLight.m_direction,
        0.0f,
        90.0f,
        1.0f,
        nearPlane,
        farPlane },
        m_hasPerspective{ true } {
    }

    glm::mat4 CameraShadowMap::getViewMatrix(
    ) const {
        return glm::lookAt(m_position, m_forward, m_worldUp);
    }

    glm::mat4 CameraShadowMap::getProjectionMatrix(
    ) const {
        if (m_hasPerspective) {
            return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
        }
        else {
            return glm::ortho(-1.0f * m_scale, 1.0f * m_scale, -1.0f * m_scale, 1.0f * m_scale, m_near, m_far);
        }
    }

    void CameraShadowMap::processMoveInput(
        CameraMovementDirection dir, 
        float deltaTime
    ) {
    }

    void CameraShadowMap::processRotateInput(
        float xOffset, 
        float yOffset, 
        float sensitivity, 
        bool constrainPitch
    ) {
    }

} // OGL