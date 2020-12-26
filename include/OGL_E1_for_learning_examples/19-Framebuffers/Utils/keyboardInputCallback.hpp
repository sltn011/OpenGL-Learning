#ifndef OGL_E1KEYBOARDCALLBACK_H
#define OGL_E1KEYBOARDCALLBACK_H

extern int postprocessMode;

namespace OGL::E1 {

    void keyboardInputCallback
    (GLFWwindow *window
    , int key
    , int scancode
    , int action
    , int mods
    ) {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            postprocessMode = (postprocessMode + 1) % 12;
        }
    }

} // OGL::E1

#endif // OGL_E1KEYBOARDCALLBACK_H