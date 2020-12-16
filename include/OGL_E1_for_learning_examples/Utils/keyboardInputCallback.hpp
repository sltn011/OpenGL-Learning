#ifndef OGL_E1KEYBOARDCALLBACK_H
#define OGL_E1KEYBOARDCALLBACK_H

namespace OGL::E1 {

    void keyboardInputCallback
    ( GLFWwindow *window
    , int key
    , int scancode
    , int action
    , int mods
    ) {
        static int depthTestCurrent = 0;
        static int depthTestFunctions[] = { GL_LESS, GL_ALWAYS, GL_NEVER, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL };

        // Switch Depth function used
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            depthTestCurrent = (depthTestCurrent + 1) % 8;
        }
    }

} // OGL::E1

#endif // OGL_E1KEYBOARDCALLBACK_H