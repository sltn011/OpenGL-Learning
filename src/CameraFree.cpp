#include "CameraFree.hpp"

namespace OGL {

    CameraFree::CameraFree( 
        glm::vec3 positionVec,
        glm::vec3 forwardVec, 
        float moveSpeed, 
        float fov,
        float aspect,
        float near,
        float far
    ) : BasicCamera { 
        positionVec,
        forwardVec, 
        moveSpeed, 
        fov, 
        aspect,
        near, 
        far }
    {
        updateVectors();
    }

    void CameraFree::processMoveInput( 
        CameraMovementDirection dir, 
        float deltaTime
    ) {
        switch (dir) {
        case CameraMovementDirection::Forward:
            m_position += m_forward * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Backward:
            m_position -= m_forward * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Left:
            m_position -= m_right * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Right:
            m_position += m_right * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Up:
            m_position += m_up * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Down:
            m_position -= m_up * m_speed * deltaTime;
            break;

        default:
            break;
        }
    }

    void CameraFree::processRotateInput( 
        float xOffset, 
        float yOffset, 
        float sensitivity, 
        bool constrainPitch
    ) {

        static bool firstTime = true;
        if (firstTime) {
            firstTime = false;
            return;
        }

        xOffset *= sensitivity;
        yOffset *= sensitivity;

        m_yaw += xOffset;
        m_pitch += yOffset;

        if (constrainPitch) {
            if (m_pitch > 89.0f) {
                m_pitch = 89.0f;
            }
            if (m_pitch < -89.0f) {
                m_pitch = -89.0f;
            }
        }
        updateVectors();
    }

} // OGL