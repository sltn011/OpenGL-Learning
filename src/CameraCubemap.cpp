#include "CameraCubemap.hpp"

namespace OGL {

    CameraCubemap::CameraCubemap
    ( glm::vec3 position
    , float near
    , float far
    ) 
    : BasicCamera
    { position
    , glm::vec3{1.0f, 0.0f, 0.0f}
    , glm::vec3{0.0f, 1.0f, 0.0f}
    , 0.0f
    , 0.0f
    , 0.0f
    , 90.0f
    , 1.0f
    , near
    , far
    } {

    }

    void CameraCubemap::setSide
    ( int side
    ) {
        switch (side) {
        case 0: // POS_X
            m_pitch = 0.0f;
            m_yaw = 0.0f;
            break;

        case 1: // NEG_X
            m_pitch = 0.0f;
            m_yaw = 180.0f;
            break;

        case 2: // POS_Y
            m_pitch = -89.99f;
            m_yaw = -90.0f;
            break;

        case 3: // NEG_Y
            m_pitch = 89.99f;
            m_yaw = -90.0f;
            break;

        case 4: // POS_Z
            m_pitch = 0.0f; 
            m_yaw = -90.0f;
            break;

        case 5: // NEG_Z
            m_pitch = 0.0f;
            m_yaw = 90.0f;
            break;

        default:
            break;
        }
        updateVectors();
    }

    void CameraCubemap::processMoveInput
    ( CameraMovementDirection dir
    , float deltaTime
    ) {
    }

    void CameraCubemap::processRotateInput
    ( float xOffset
    , float yOffset
    , float sensitivity
    , bool constrainPitch
    ) {
    }


} // OGL