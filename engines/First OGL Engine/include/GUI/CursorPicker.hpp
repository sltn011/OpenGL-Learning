#ifndef OGL_E1_GUI_CURSORPICKER
#define OGL_E1_GUI_CURSORPICKER

#include "FrameBufferObject.hpp"
#include "Object.hpp"
#include <unordered_map>
#include "GLFW/glfw3.h"

namespace OGL::E1::GUI {

    class CursorPicker {
     public:

         Object *getSelected(
             FrameBufferObject &fbo,
             std::unordered_map<int, Object*> &colorObjMap,
             GLFWwindow *window
         );

         int rgbColorToInt(
             int r,
             int g,
             int b
         ) const;

    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_CURSORPICKER
