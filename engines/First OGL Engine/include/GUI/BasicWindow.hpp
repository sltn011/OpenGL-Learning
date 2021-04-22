#ifndef OGL_E1_GUI_BASICWINDOW
#define OGL_E1_GUI_BASICWINDOW

#include "imgui.h"

namespace OGL::E1::GUI {

    class BasicWindow {
     public:
        BasicWindow(
        );

        BasicWindow(
            bool isEnabled
        );

        virtual ~BasicWindow(
        );

        virtual void render(
        ) = 0;

     public:
         bool m_enabled;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_BASICWINDOW
