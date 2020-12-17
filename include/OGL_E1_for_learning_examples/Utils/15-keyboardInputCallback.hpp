#ifndef OGL_E1KEYBOARDCALLBACK_H
#define OGL_E1KEYBOARDCALLBACK_H

extern int shaderNum;

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
            glDepthFunc(depthTestFunctions[depthTestCurrent]);
            switch (depthTestCurrent)
            {
            case 0:
                printf("GL_LESS\n");
                break;

            case 1:
                printf("GL_ALWAYS\n");
                break;

            case 2:
                printf("GL_NEVER\n");
                break;

            case 3:
                printf("GL_EQUAL\n");
                break;

            case 4:
                printf("GL_LEQUAL\n");
                break;

            case 5:
                printf("GL_GREATER\n");
                break;

            case 6:
                printf("GL_NOTEQUAL\n");
                break;

            case 7:
                printf("GL_GEQUAL\n");
                break;

            default:
                break;
            }
        }

        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
            shaderNum ^= 1;
        }
    }

} // OGL::E1

#endif // OGL_E1KEYBOARDCALLBACK_H