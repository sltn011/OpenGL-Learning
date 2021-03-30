#ifndef OGL_CAMERAFIRSTPERSON_H
#define OGL_CAMERAFIRSTPERSON_H

#include "BasicCamera.hpp"

namespace OGL {

    class CameraFirstPerson : public BasicCamera {
     public:
        CameraFirstPerson( 
            glm::vec3 positionVec, 
            glm::vec3 forwardVec, 
            glm::vec3 worldUpVec, 
            float moveSpeed, 
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

#endif // OGL_CAMERAFIRSTPERSON_H