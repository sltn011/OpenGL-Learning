#ifndef OGL_E1MOUSECALLBACK_H
#define OGL_E1MOUSECALLBACK_H

#include "GameCamera.hpp"

namespace OGL::E1 {

    void mouseInputCallback(
        GLFWwindow *window,
        double xpos, 
        double ypos
    ) {
        float xOffset = static_cast<float>(xpos) - System::lastMouseXPos;
        float yOffset = System::lastMouseYPos - static_cast<float>(ypos);
        System::lastMouseXPos = static_cast<float>(xpos);
        System::lastMouseYPos = static_cast<float>(ypos);
        
        GameCamera::inst->processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
    }

} // OGL::E1

#endif // OGL_E1MOUSECALLBACK_H