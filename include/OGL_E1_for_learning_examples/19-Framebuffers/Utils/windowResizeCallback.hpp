#ifndef OGL_E1WINDOWRESIZECALLBACK_H
#define OGL_E1WINDOWRESIZECALLBACK_H

namespace OGL::E1 {

    void windowResizeCallback( 
        GLFWwindow *window, 
        int newWidth,
        int newHeight
    ) {
        glViewport(0, 0, newWidth, newHeight);
    }

} // OGL::E1

#endif // OGL_E1WINDOWRESIZECALLBACK_H