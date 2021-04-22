#ifndef OGL_E1_GUI_GUIRENDERER
#define OGL_E1_GUI_GUIRENDERER

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GUI/MainWindow.hpp"
#include "GUI/ObjectTransformWindow.hpp"

#include <string>
#include <unordered_map>
#include <memory>

#include "GLFW/glfw3.h"

namespace OGL::E1::GUI {

    class GUIRenderer {
     public:
         GUIRenderer(
             GLFWwindow *window,
             std::string glslVersion
         );

         ~GUIRenderer(
         );

         GUIRenderer(
             GUIRenderer const &rhs
         ) = delete;

         GUIRenderer(
             GUIRenderer &&rhs
         ) = default;

         GUIRenderer &operator=(
             GUIRenderer const &rhs
         ) = delete;

         GUIRenderer &operator=(
             GUIRenderer &&rhs
         ) = default;

         std::unordered_map<WindowsType, std::unique_ptr<BasicWindow>> &getWindows(
         );

         void addWindow(
             WindowsType windowType,
             std::unique_ptr<BasicWindow> window
         );

         void render(
         );

     protected:
         void renderMainWindow(
         );

         void renderSubWindows(
         );

         void handleUserActivity(
         );

         std::unique_ptr<BasicWindow> createWindow(
             WindowsType windowType,
             bool isEnabled
         ) const;

     protected:
         MainWindow m_mainWindow;
         std::unordered_map<WindowsType, std::unique_ptr<BasicWindow>> m_windows;

    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_GUIRENDERER
