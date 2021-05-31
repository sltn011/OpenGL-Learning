/**
* @file CursorPicker.hpp
*/

#ifndef OGL_E1_GUI_CURSORPICKER
#define OGL_E1_GUI_CURSORPICKER

#include "FrameBufferObject.hpp"
#include "Object.hpp"
#include <unordered_map>
#include "GLFW/glfw3.h"

namespace OGL::E1::GUI {

    /**
     * @brief Class for selecting Object pointed by cursor
    */
    class CursorPicker {
     public:
        /**
         * @brief Samples color in FrameBufferObject at cursor's position and uses it to get pointed Object 
         * @param fbo FrameBufferObject with scene rendered using ColoredShapesRenderer 
         * @param colorObjMap Hashmap with Key - Objects's shape color encoded as R + 1000 * G + 1000000 * B and Value - pointer to Object with this color
         * @param window Pointer to game window
         * @return Pointer to Object selected with cursor or nullptr if empty space was clicked
        */
        Object *getSelected(
            FrameBufferObject &fbo,
            std::unordered_map<int, Object*> &colorObjMap,
            GLFWwindow *window
        );

        /**
         * @brief Converts RGB color into it's integer color code: Code = R + 1000 * G + 1000000 * B
         * @param r Color's red value
         * @param g Color's green value
         * @param b Color's blue value
         * @return Integer color code: Code = R + 1000 * G + 1000000 * B
        */
        int rgbColorToInt(
            int r,
            int g,
            int b
        ) const;

    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_CURSORPICKER
