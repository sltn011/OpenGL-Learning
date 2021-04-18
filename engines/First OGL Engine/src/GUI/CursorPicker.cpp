#include "GUI/CursorPicker.hpp"

namespace OGL::E1::GUI {

    Object *CursorPicker::getSelected(
        FrameBufferObject &fbo,
        std::unordered_map<int, Object*> &colorObjMap,
        GLFWwindow *window
    ) {
        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        double cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        cursorY = screenHeight - cursorY;

        fbo.bind(GL_FRAMEBUFFER);
        glm::vec3 pickedColor{};
        glReadPixels((int)cursorX, (int)cursorY, 1, 1, GL_RGB, GL_FLOAT, &(pickedColor.x));
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
        pickedColor *= 255;

        int colorCode = rgbColorToInt((int)pickedColor.r, (int)pickedColor.g, (int)pickedColor.b);
        auto maybeObject = colorObjMap.find(colorCode);
        if (maybeObject == colorObjMap.end()) {
            return nullptr;
        }
        else {
            return maybeObject->second;
        }
    }

    int CursorPicker::rgbColorToInt(
        int r,
        int g,
        int b
    ) const {
        return r + 1000 * g + 1000000 * b;
    }

} // OGL::E1::GUI