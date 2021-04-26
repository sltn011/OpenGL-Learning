#ifndef OGL_CAMERASTATIC_H
#define OGL_CAMERASTATIC_H

#include "BasicCamera.hpp"

namespace OGL {

    class CameraStatic : public BasicCamera {
    public:
        CameraStatic(
            glm::vec3 positionVec,
            glm::vec3 forwardVec,
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

#endif // OGL_CAMERASTATIC_H