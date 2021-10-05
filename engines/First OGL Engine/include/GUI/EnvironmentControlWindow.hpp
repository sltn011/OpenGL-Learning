/**
* @file EnvironmentControlWindow.hpp
*/

#ifndef OGL_E1_GUI_ENVIRONMENTCONTROLWINDOW
#define OGL_E1_GUI_ENVIRONMENTCONTROLWINDOW

#include "GUI/BasicWindow.hpp"
#include "Object.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/UtilsLibrary.hpp"

namespace OGL::E1::GUI {

    /**
     * @brief GUI window for re-rendering shadows and reflections
    */
    class EnvironmentControlWindow : public BasicWindow {

     public:
        /**
         * @brief EnvironmentControlWindow constructor
         * @param isEnabled isEnabled Flag which indicates if window should be rendered or not
        */
        explicit EnvironmentControlWindow(
            bool isEnabled = false
        );

        /**
         * @brief Renders EnvironmentControlWindow
        */
        void render(
        ) override;

        Delegate<> m_OnRebuildShadows;
        Delegate<> m_OnRebuildReflections;

    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_ENVIRONMENTCONTROLWINDOW
