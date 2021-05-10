/**
* @file BasicCamera.hpp
*/

#ifndef OGL_BASICCAMERA_H
#define OGL_BASICCAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace OGL {

    /**
     * @brief Camera Movement Direction enum
    */
    enum class CameraMovementDirection {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    /**
     * @brief Abstract basic class for other Camera classes implementations
    */
    class BasicCamera {
     protected:
        /// Position of camera in world space
        glm::vec3 m_position;

        /// Unit vector of camera's look direction
        glm::vec3 m_forward;

        /// Unit vector that points to the right of camera
        glm::vec3 m_right;

        /// Unit vector that points to the up of camera
        glm::vec3 m_up;

        /// Unit vector that points to the world up
        glm::vec3 m_worldUp;

        /// Camera's yaw rotation(around camera up vector)
        float m_yaw; 

        /// Camera's pitch rotation(around camera right vector)
        float m_pitch;

     public:
        /// Camera's field of view in angles
        float m_fov;

        /// Camera's aspect ratio (width / height)
        float m_aspect;

        /// Camera's near clip plane
        float m_near;

        /// Camera's far clip plane
        float m_far;

        /// Camera's speed multiplier
        float m_speed;

     public:

        /**
         * @brief BasicCamera class constructor
         * @param positionVec Position in world space
         * @param forwardVec Unit vector of camera's look direction
         * @param moveSpeed Movement speed multiplier
         * @param fov Field of view in degrees
         * @param aspect Aspect ratio (width/height)
         * @param near Near clip plane
         * @param far Far clip plane
        */
        BasicCamera(
            glm::vec3 positionVec,
            glm::vec3 forwardVec,
            float moveSpeed,
            float fov,
            float aspect,
            float near = 0.1f,
            float far = 100.0f
        );

        /**
         * @brief Virtual camera destructor
        */
        virtual ~BasicCamera(
        );

        /**
         * @brief Virtual get method of camera's view matrix
         * @return 4x4 view matrix
        */
        virtual glm::mat4 getViewMatrix(
        ) const;

        /**
         * @brief Virtual get method of camera's projection matrix
         * @return 4x4 projection matrix
        */
        virtual glm::mat4 getProjectionMatrix(
        ) const;

        /**
         * @brief Position getter
         * @return Position in world space
        */
        glm::vec3 getPos(
        ) const;

        /**
         * @brief Look direction vector getter
         * @return Look direction vector
        */
        glm::vec3 getForward(
        ) const;

        /**
         * @brief World space up getter
         * @return World space up
        */
        glm::vec3 getWorldUp(
        ) const;

        /**
         * @brief Up vector getter
         * @return Up vector
        */
        glm::vec3 getUp(
        ) const;

        /**
         * @brief Right vector getter
         * @return Right vector
        */
        glm::vec3 getRight(
        ) const;

        /**
         * @brief Get camera's yaw rotation in angles
         * @return Yaw rotation angles
        */
        float getYaw(
        ) const;

        /**
         * @brief Get camera's pitch rotation in angles
         * @return Pitch rotation angles
        */
        float getPitch(
        ) const;

        /**
         * @brief Abstract method to handle camera's movement input
         * @param dir Movement direction
         * @param deltaTime Time between this frame and previous one in seconds
        */
        virtual void processMoveInput(
            CameraMovementDirection dir,
            float deltaTime
        ) = 0;

        /**
         * @brief Abstract method to handle camera's rotation input
         * @param xOffset Delta of X rotation(from left to right)
         * @param yOffset Delta of Y rotation(from bottom to top)
         * @param sensitivity Rotation delta's multiplier
         * @param constrainPitch If set - should limit pitch from reaching +-90.0f
        */
        virtual void processRotateInput(
            float xOffset,
            float yOffset, 
            float sensitivity, 
            bool constrainPitch
        ) = 0;

     protected:
        /**
         * @brief Recalculates camera's direction vectors
        */
        virtual void updateVectors(
        );
    };

} // OGL

#endif // OGL_BASICCAMERA_H
