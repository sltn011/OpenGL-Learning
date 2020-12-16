#include "CameraFirstPerson.hpp"

namespace OGL {

    CameraFirstPerson::CameraFirstPerson
    (glm::vec3 positionVec
    , glm::vec3 forwardVec
    , glm::vec3 worldUpVec
    , float moveSpeed
    , float yawAngle
    , float pitchAngle
    )
    : BasicCamera
    { positionVec
    , forwardVec
    , worldUpVec
    , moveSpeed
    , yawAngle
    , pitchAngle
    }
    {
        updateVectors();
    }

    void CameraFirstPerson::processMoveInput
    ( CameraMovementDirection dir
    , float deltaTime
    ) {
        glm::vec3 forward = m_forward;
        forward.y = m_position.y;
        forward = glm::normalize(forward);
        switch (dir) {
        case CameraMovementDirection::Forward:
            m_position += forward * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Backward:
            m_position -= forward * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Left:
            m_position -= m_right * m_speed * deltaTime;
            break;

        case CameraMovementDirection::Right:
            m_position += m_right * m_speed * deltaTime;
            break;

        default:
            break;
        }
    }

    void CameraFirstPerson::processRotateInput
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

} // OGL