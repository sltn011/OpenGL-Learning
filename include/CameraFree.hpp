#ifndef OGL_CAMERAFREE_H
#define OGL_CAMERAFREE_H

#include "BasicCamera.hpp"

namespace OGL {

    class CameraFree : public BasicCamera {
     public:
        CameraFree( 
            glm::vec3 positionVec, 
            glm::vec3 forwardVec, 
            glm::vec3 worldUpVec, 
            float moveSpeed, 
            float yawAngle, 
            float pitchAngle,
            float fov, 
            float aspect,
            float near, 
            float far
        );

        void processMoveInput( 
            CameraMovementDirection dir,
            float deltaTime
        ) override;

        void processRotateInput( 
            float xOffset,
            float yOffset, 
            float sensitivity,
            bool constrainPitch
        ) override;

    };

} // OGL

#endif // OGL_CAMERAFREE_H