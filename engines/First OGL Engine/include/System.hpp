/**
* @file System.hpp
*/

#ifndef OGL_E1SYSTEM_H
#define OGL_E1SYSTEM_H

namespace OGL::E1 {

    /**
     * @brief Struct that contains System data used by Engine
    */
    struct System {
        /// Time in seconds between this frame and previous one
        float deltaTime = 0.0f;

        /// Time in seconds of previous frame being rendered
        float lastFrameTime = 0.0f;


        /// X coordinate of cursor's position on screen from left to right
        float lastMouseXPos = 0.0f;

        /// Y coordinate of cursor's position on screen from top to bottom
        float lastMouseYPos = 0.0f;


        /// Mouse sensitivity
        float mouseSensitivity = 0.15f;

    };

} // OGL::E1

#endif // OGL_E1SYSTEM_H