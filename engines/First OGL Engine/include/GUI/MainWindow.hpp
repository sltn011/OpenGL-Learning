/**
* @file MainWindow.hpp
*/

#ifndef OGL_E1_GUI_MAINWINDOW
#define OGL_E1_GUI_MAINWINDOW

#include "GUI/BasicWindow.hpp"
#include "GUI/WindowsTypes.hpp"

namespace OGL::E1::GUI {

    /**
     * @brief Main GUI window
    */
    class MainWindow : public BasicWindow {

     private:
        /// Flag which indicates if there was any user activity in MainWindow
        bool         m_hadUserActivity;

        /// Type of Window to be changed or WindowsType::NONE if no window needs changes
        WindowsTypes m_changedWindow;

        /// Index of selected GUI Window in Windows dropdown list
        int          m_windowsDropDownIndex;

     public:
        /**
         * @brief MainWindow constructor
         * @param isEnabled Flag which indicates if window should be rendered or not
        */
        explicit MainWindow(
            bool isEnabled = false
        );
        
        /**
         * @brief Renders MainWindow
        */
        void render(
        ) override;
        
        /**
         * @brief Checks if there were any actions with MainWindow from user  
         * @return True if there were any actions, False otherwise
        */
        bool hadUserActivity(
        ) const;
        
        /**
         * @brief Tells which GUI window needs to be changed(created, shown, hidden)
         * @return Type of Window to be changed or WindowsType::NONE if no window needs changes
        */
        WindowsTypes getChangedWindow(
        ) const;

    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_MAINWINDOW
