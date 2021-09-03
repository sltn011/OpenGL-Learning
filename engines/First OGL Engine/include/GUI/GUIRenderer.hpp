/**
* @file GUIRenderer.hpp
*/

#ifndef OGL_E1_GUI_GUIRENDERER
#define OGL_E1_GUI_GUIRENDERER

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GUI/MainWindow.hpp"
#include "GUI/ObjectTransformWindow.hpp"
#include "GUI/EnvironmentControlWindow.hpp"

#include <string>
#include <unordered_map>
#include <memory>

#include "GLFW/glfw3.h"

namespace OGL::E1::GUI {

    /**
     * @brief Class for rendering GUI windows
    */
    class GUIRenderer {
     protected:
        /// GUI's MainWindow
        MainWindow m_mainWindow;

        /// Hashmap wth Key: GUI window type, Value: Pointer to GUI window
        std::unordered_map<WindowsTypes, std::unique_ptr<BasicWindow>> m_windows;

        // Number of instances of this class - used when allocating and deallocating ImGui resources
        static int s_numberOfInstances;

     public:
        /**
         * @brief GUIRenderer constructor
         * @param window Pointer to game window where GUI will be rendered
         * @param glslVersion String with format "#version N", where N is a GLSL version
        */
        GUIRenderer(
            GLFWwindow *window,
            std::string glslVersion
        );

        /**
         * @brief GUIRenderer destructor
        */
        ~GUIRenderer(
        );

        GUIRenderer(
            GUIRenderer const &rhs
        ) = delete;

        GUIRenderer(
            GUIRenderer &&rhs
        ) noexcept;

        GUIRenderer &operator=(
            GUIRenderer const &rhs
        ) = delete;

        GUIRenderer &operator=(
            GUIRenderer &&rhs
        ) = default;

        /**
         * @brief Get Hashmap of GUI windows
         * @return Hashmap wth Key: GUI window type, Value: Pointer to GUI window
        */
        std::unordered_map<WindowsTypes, std::unique_ptr<BasicWindow>> &getWindows(
        );

        /**
         * @brief Adds new GUI window
         * @param windowType Type of new GUI window
         * @param window Pointer to new GUI window
        */
        void addWindow(
            WindowsTypes windowType,
            std::unique_ptr<BasicWindow> window
        );

        /**
         * @brief Handles GUI rendering and interaction
        */
        void render(
        );

     protected:
        /**
         * @brief Renders GUI's MainWindow
        */
        void renderMainWindow(
        );

        /**
         * @brief Renders all created and enabled subwindows
        */
        void renderSubWindows(
        );

        /**
         * @brief Checks if MainWindow had any user activity(creating, hiding, showing subwindows) and handles it
        */
        void handleUserActivity(
        );

        /**
         * @brief Creates new GUI Window
         * @param windowType Type of new GUI Window
         * @param isEnabled Flag which indicates if window should be rendered or not
         * @return Pointer to new GUI Window
        */
        std::unique_ptr<BasicWindow> createWindow(
            WindowsTypes windowType,
            bool isEnabled
        ) const;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_GUIRENDERER
