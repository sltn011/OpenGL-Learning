#ifndef OGL_E1_GUI_MAINWINDOW
#define OGL_E1_GUI_MAINWINDOW

#include "GUI/BasicWindow.hpp"
#include "GUI/WindowsTypes.hpp"

namespace OGL::E1::GUI {

    class MainWindow : public BasicWindow {
     public:
        MainWindow(
            bool isEnabled = false
        );

        void render(
        ) override;

        bool hadUserActivity(
        ) const;

        WindowsType getChangedWindow(
        ) const;

     private:
        int         m_windowsDropDownIndex;
        bool        m_hadUserActivity;
        WindowsType m_changedWindow;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_MAINWINDOW
