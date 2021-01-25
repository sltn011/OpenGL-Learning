#ifndef OGL_E1KEYBOARDCALLBACK_H
#define OGL_E1KEYBOARDCALLBACK_H

namespace OGL::E1 {

    void keyboardInputCallback(
        GLFWwindow *window, 
        int key, 
        int scancode,
        int action,
        int mods
    ) {
        static int funcs[] = { GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX };
        static int funcIndex = 0;

        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            funcIndex = (funcIndex + 1) % 5;
            glBlendEquation(funcs[funcIndex]);

            switch (funcIndex) {
            case 0:
                printf("GL_FUNC_ADD\n");
                break;

            case 1:
                printf("GL_FUNC_SUBTRACT\n");
                break;

            case 2:
                printf("GL_FUNC_REVERSE_SUBTRACT\n");
                break;

            case 3:
                printf("GL_MIN\n");
                break;

            case 4:
                printf("GL_MAX\n");
                break;

            default:
                break;
            }
        }
    }

} // OGL::E1

#endif // OGL_E1KEYBOARDCALLBACK_H