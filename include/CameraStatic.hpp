/**
* @file CameraStatic.hpp
*/

#ifndef OGL_CAMERASTATIC_H
#define OGL_CAMERASTATIC_H

#include "BasicCamera.hpp"

namespace OGL {

    /**
     * @brief Camera that can't move or rotate by user input
    */
    class CameraStatic : public BasicCamera {
    public:

        /**
         * @brief CameraStatic constructor
         * @param positionVec Position in world space
         * @param forwardVec Unit vector of camera's look direction
         * @param fov Field of view in degrees
         * @param aspect Aspect ratio (width/height)
         * @param near Near clip plane
         * @param far Far clip plane
        */
        CameraStatic(
            glm::vec3 positionVec,
            glm::vec3 forwardVec,
            float fov,
            float aspect,
            float near,
            float far
        );

        /**
         * @brief Override of BasicCamera move process method - does nothing
         * @param dir unused value
         * @param deltaTime unused value
        */
        void processMoveInput(
            CameraMovementDirection dir,
            float deltaTime
        ) override;

        /**
         * @brief Override of BasicCamera rotation process method - does nothing
         * @param xOffset unused value
         * @param yOffset unused value
         * @param sensitivity unused value
         * @param constrainPitch unused value
        */
        void processRotateInput(
            float xOffset,
            float yOffset,
            float sensitivity,
            bool constrainPitch
        ) override;

    };

} // OGL

#endif // OGL_CAMERASTATIC_H