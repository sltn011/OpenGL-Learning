#ifndef OGL_CAMERAFREE_H
#define OGL_CAMERAFREE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace OGL {

    enum CameraMovementDirection {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    class CameraFree {
        glm::vec3 m_position;
        glm::vec3 m_forward;
        glm::vec3 m_right;
        glm::vec3 m_up;
        glm::vec3 m_worldUp;
        float m_speed;
        float m_yaw;
        float m_pitch;

    public:
        CameraFree
        ( glm::vec3 positionVec
        , glm::vec3 forwardVec
        , glm::vec3 worldUpVec
        , float moveSpeed
        , float yawAngle
        , float pitchAngle
        );

        glm::mat4 getViewMatrix
        (
        ) const;

        glm::vec3 getPos
        (
        ) const;

        glm::vec3 getForward
        (
        ) const;

        glm::vec3 getUp
        (
        ) const;

        void processMoveInput
        ( CameraMovementDirection dir
        , float deltaTime
        );

        void processRotateInput
        ( float xOffset
        , float yOffset
        , float sensitivity
        , bool constrainPitch
        );

    private:
        void updateVectors
        (
        );
    };

} // OGL

#endif // OGL_CAMERAFREE_H