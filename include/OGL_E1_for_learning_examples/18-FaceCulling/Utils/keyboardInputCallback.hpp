#ifndef OGL_E1KEYBOARDCALLBACK_H
#define OGL_E1KEYBOARDCALLBACK_H

namespace OGL::E1 {

    void keyboardInputCallback
    (GLFWwindow *window
    , int key
    , int scancode
    , int action
    , int mods
    ) {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            static int toCull[] = { GL_BACK, GL_FRONT, GL_FRONT_AND_BACK };
            static int toCullIndex = 0;
            toCullIndex = (toCullIndex + 1) % 3;
            glCullFace(toCull[toCullIndex]);
            switch (toCullIndex) {
            case 0:
                printf("GL_BACK\n");
                break;

            case 1:
                printf("GL_FRONT\n");
                break;

            case 2:
                printf("GL_FRONT_AND_BACK\n");
                break;

            default:
                break;
            }
        }

        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
            static int winding[] = { GL_CCW, GL_CW };
            static int windingIndex = 0;
            windingIndex = (windingIndex + 1) % 2;
            glFrontFace(winding[windingIndex]);
            switch (windingIndex) {
            case 0:
                printf("GL_CCW\n");
                break;

            case 1:
                printf("GL_CW\n");
                break;

            default:
                break;
            }
        }
    }

} // OGL::E1

#endif // OGL_E1KEYBOARDCALLBACK_H