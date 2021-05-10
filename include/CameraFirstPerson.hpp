/**
* @file CameraFirstPerson.hpp
*/

#ifndef OGL_CAMERAFIRSTPERSON_H
#define OGL_CAMERAFIRSTPERSON_H

#include "BasicCamera.hpp"

namespace OGL {

    /**
     * @brief Camera class that cant change it's Y position coord
    */
    class CameraFirstPerson : public BasicCamera {
     public:

         /**
         * @brief CameraFirstPerson constructor
         * @param positionVec Position in world space
         * @param forwardVec Unit vector of camera's look direction
         * @param moveSpeed Movement speed multiplier
         * @param fov Field of view in degrees
         * @param aspect Aspect ratio (width/height)
         * @param near Near clip plane
         * @param far Far clip plane
        */
        CameraFirstPerson( 
            glm::vec3 positionVec, 
            glm::vec3 forwardVec, 
            float moveSpeed, 
            float fov,
            float aspect, 
            float near, 
            float far
        );

        /**
         * @brief Override of BasicCamera method to handle camera's movement input
         * @param dir Movement direction
         * @param deltaTime Time between this frame and previous one in seconds
        */
        void processMoveInput( 
            CameraMovementDirection dir, 
            float deltaTime
        ) override;

        /**
         * @brief Override of BasicCamera method to handle camera's rotation input
         * @param xOffset Delta of X rotation(from left to right)
         * @param yOffset Delta of Y rotation(from bottom to top)
         * @param sensitivity Rotation delta's multiplier
         * @param constrainPitch If set - should limit pitch from reaching +-90.0f
        */
        void processRotateInput( 
            float xOffset,
            float yOffset, 
            float sensitivity,
            bool constrainPitch
        ) override;

    };

} // OGL

#endif // OGL_CAMERAFIRSTPERSON_H