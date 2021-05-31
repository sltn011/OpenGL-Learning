/**
* @file CameraCubemap.hpp
*/

#ifndef OGL_CAMERACUBEMAP_H
#define OGL_CAMERACUBEMAP_H

#include "BasicCamera.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace OGL {

    /**
     * @brief Camera class to render cubemap
    */
    class CameraCubemap : public BasicCamera {
     public:

        /**
         * @brief CameraCubemap constructor
         * @param position Camera position in world space
         * @param near Near clip plane
         * @param far Far clip plane
        */
        CameraCubemap( 
            glm::vec3 position, 
            float near, 
            float far
        );

        /**
         * @brief Rotates camera to face cubemap's side
         * @param side Cubemap side value (0-5) - (+X, -X, +Y, -Y, +Z, -Z)
        */
        void setSide( 
            int side
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
}

#endif // OGL_CAMERACUBEMAP_H