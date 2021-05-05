#ifndef OGL_BASICCAMERA_H
#define OGL_BASICCAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace OGL {

    enum class CameraMovementDirection {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    class BasicCamera {
     protected:
        glm::vec3 m_position;
        glm::vec3 m_forward;
        glm::vec3 m_right;
        glm::vec3 m_up;
        glm::vec3 m_worldUp;
        float m_yaw;    // Y rotation
        float m_pitch;  // X rotation

     public:
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        float m_speed;

     public:
        BasicCamera(
            glm::vec3 positionVec,
            glm::vec3 forwardVec,
            float moveSpeed,
            float fov,
            float aspect,
            float near = 0.1f,
            float far = 100.0f
        );

        virtual ~BasicCamera(
        );

        virtual glm::mat4 getViewMatrix(
        ) const;

        virtual glm::mat4 getProjectionMatrix(
        ) const;

        glm::vec3 getPos(
        ) const;

        glm::vec3 getForward(
        ) const;

        glm::vec3 getWorldUp(
        ) const;

        glm::vec3 getUp(
        ) const;

        glm::vec3 getRight(
        ) const;

        float getYaw(
        ) const;

        float getPitch(
        ) const;

        virtual void processMoveInput(
            CameraMovementDirection dir,
            float deltaTime
        ) = 0;

        virtual void processRotateInput(
            float xOffset,
            float yOffset, 
            float sensitivity, 
            bool constrainPitch
        ) = 0;

     protected:
        virtual void updateVectors(
        );
    };

} // OGL

#endif // OGL_BASICCAMERA_H
