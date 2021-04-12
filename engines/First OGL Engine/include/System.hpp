#ifndef OGL_E1SYSTEM_H
#define OGL_E1SYSTEM_H

namespace OGL::E1 {

    struct System {
        float deltaTime = 0.0f;
        float lastFrameTime = 0.0f;

        float lastMouseXPos = 0.0f;
        float lastMouseYPos = 0.0f;

        float mouseSensitivity = 0.15f;

    };

} // OGL::E1

#endif // OGL_E1SYSTEM_H