#ifndef OGL_CAMERACUBEMAP_H
#define OGL_CAMERACUBEMAP_H

#include "BasicCamera.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace OGL {
    class CameraCubemap : public BasicCamera {
    protected:
        glm::mat4 m_view;
        glm::mat4 m_projection;

    public:
        CameraCubemap
        ( glm::vec3 position
        , float near
        , float far
        );

        glm::mat4 getProjectionMatrix
        (
        ) const;

        void setSide
        ( int side
        );

        void processMoveInput
        ( CameraMovementDirection dir
        , float deltaTime
        ) override;

        void processRotateInput
        ( float xOffset
        , float yOffset
        , float sensitivity
        , bool constrainPitch
        ) override;

    };
}

#endif // OGL_CAMERACUBEMAP_H