#include "..\include\CameraFree.hpp"
#include "CameraFree.hpp"

namespace OGL {

    CameraFree::CameraFree
    ( glm::vec3 positionVec
    , glm::vec3 forwardVec
    , glm::vec3 worldUpVec
    , float moveSpeed
    , float yawAngle
    , float pitchAngle
    ) 
    : m_position{positionVec}
    , m_forward{forwardVec}
    , m_worldUp{worldUpVec}
    , m_speed{moveSpeed}
    , m_yaw{yawAngle}
    , m_pitch{pitchAngle}
    {
        updateVectors();
    }

    glm::mat4 CameraFree::getViewMatrix
    (
    ) const {
        return glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    glm::vec3 CameraFree::getPos
    (
    ) const {
        return m_position;
    }

    glm::vec3 CameraFree::getForward
    (
    ) const {
        return m_forward;
    }

    glm::vec3 CameraFree::getUp
    (
    ) const {
        return m_up;
    }

    void CameraFree::processMoveInput
    ( CameraMovementDirection dir
    , float deltaTime
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
        }
    }

    void CameraFree::processRotateInput
    ( float xOffset
    , float yOffset
    , float sensitivity
    , bool constrainPitch
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

    void CameraFree::updateVectors
    (
    ) {
        m_forward.x = std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));
        m_forward.y = std::sin(glm::radians(m_pitch));
        m_forward.z = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));
        m_forward = glm::normalize(m_forward);

        m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }

} // OGL