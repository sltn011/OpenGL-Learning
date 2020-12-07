#ifndef OGL_E1GAMECAMERA_H
#define OGL_E1GAMECAMERA_H

#include "CameraFirstPerson.hpp"
#include "CameraFree.hpp"
#include "EngineTypes.hpp"
#include "System.hpp"

namespace OGL::E1 {

    namespace GameCamera {
        smartCamPtr cam;

        void processCamInput
        ( GLFWwindow *window
        ) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                cam->processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                cam->processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                cam->processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                cam->processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                cam->processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                cam->processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime);
            }
        }
    }

} // OGL::E1

#endif // OGL_E1GAMECAMERA_H