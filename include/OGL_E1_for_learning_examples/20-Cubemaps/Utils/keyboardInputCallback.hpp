#ifndef OGL_E1KEYBOARDCALLBACK_H
#define OGL_E1KEYBOARDCALLBACK_H

extern int playgroundRenderMode;
extern int crateRenderMode;
extern int windowRenderMode;

const int numModes = 4;

namespace OGL::E1 {

    void keyboardInputCallback(
        GLFWwindow *window,
        int key, 
        int scancode,
        int action, 
        int mods
    ) {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            playgroundRenderMode = (playgroundRenderMode + 1) % numModes;
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
            crateRenderMode = (crateRenderMode + 1) % numModes;
        }
        if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
            windowRenderMode = (windowRenderMode + 1) % numModes;
        }
    }

} // OGL::E1

#endif // OGL_E1KEYBOARDCALLBACK_H