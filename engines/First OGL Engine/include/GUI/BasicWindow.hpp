/**
* @file BasicWindow.hpp
*/

#ifndef OGL_E1_GUI_BASICWINDOW
#define OGL_E1_GUI_BASICWINDOW

#include "imgui.h"

namespace OGL::E1::GUI {
    
    /**
     * @brief Abstract class for creating GUI windows
    */
    class BasicWindow {
     public:
        /// Flag which indicates if window should be rendered or not
        bool m_enabled;

     public:
        /**
         * @brief BasicWindow constructor
         * @param isEnabled Flag which indicates if window should be rendered or not
        */
        explicit BasicWindow(
            bool isEnabled = false
        );
        
        /**
         * @brief Basic window destructor
        */
        virtual ~BasicWindow(
        );
        
        /**
         * @brief Abstract method used to render window on the screen
        */
        virtual void render(
        ) = 0;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_BASICWINDOW
