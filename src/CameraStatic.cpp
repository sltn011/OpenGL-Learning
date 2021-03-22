#include "CameraStatic.hpp"

namespace OGL {

    CameraStatic::CameraStatic(
        glm::vec3 positionVec, 
        glm::vec3 forwardVec,
        glm::vec3 worldUpVec,
        float fov,
        float aspect,
        float near,
        float far
    ) : BasicCamera {
        positionVec,
        forwardVec,
        worldUpVec,
        0.0f,
        0.0f,
        0.0f,
        fov,
        aspect,
        near,
        far
    } {
        m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }

    void CameraStatic::processMoveInput(
        CameraMovementDirection dir, 
        float deltaTime
    ) {
    }

    void CameraStatic::processRotateInput(
        float xOffset, 
        float yOffset,
        float sensitivity,
        bool constrainPitch
    ) {

    }

} // OGL