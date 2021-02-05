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
        {0.0f, 1.0f, 0.0f},
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        nearPlane,
        farPlane },
        m_scale {sceneDownscale} {
    }

    glm::mat4 CameraShadowMap::getViewMatrix(
    ) const {
        return glm::lookAt(m_position, m_forward, m_worldUp);
    }

    glm::mat4 CameraShadowMap::getProjectionMatrix(
    ) const {
        return glm::ortho(-1.0f * m_scale, 1.0f * m_scale, -1.0f * m_scale, 1.0f * m_scale, m_near, m_far);
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