#ifndef OGL_E1_H
#define OGL_E1_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "CameraFirstPerson.hpp"
#include "Object.hpp"

#include "Utils/EngineTypes.hpp"
#include "Utils/Events.hpp"
#include "Utils/callbacks.hpp"
#include "Utils/GameCamera.hpp"
#include "Utils/System.hpp"
#include "Utils/factory.hpp"

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <queue>
#include <memory>
#include <unordered_map>
#include <vector>
#include <atomic>

namespace OGL::E1 {

    class Engine1Base {
    public:
        Engine1Base
        ( int          screenWidth
        , int          screenHeight
        , std::string  title = "Engine1_v.0.1"
        , bool         isWindowed = true
        ) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            GLFWmonitor *monitor = isWindowed ? nullptr : glfwGetPrimaryMonitor();

            GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), monitor, nullptr);
            if (!window) {
                throw OGL::Exception("Error creating window!");
            }
            m_window = window;

            System::lastMouseXPos = screenWidth / 2.0f;
            System::lastMouseYPos = screenHeight / 2.0f;
            
            m_title = title;

            glfwMakeContextCurrent(m_window);
            glfwSetFramebufferSizeCallback(m_window, windowResizeCallback);
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetKeyCallback(m_window, keyboardInputCallback);
            glfwSetCursorPosCallback(m_window, mouseInputCallback);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                throw OGL::Exception("Error loading GLAD!");
            }
        }

        virtual ~Engine1Base(){
            glfwDestroyWindow(m_window);
        }

        void start
        (
        ) {
            m_gameShouldRun = true;
            if (!userCreate()) {
                m_gameShouldRun = false;
                return;
            }
            System::lastFrameTime = (float)glfwGetTime();
            while (m_gameShouldRun) {
                float currentFrameTime = (float)glfwGetTime();
                System::deltaTime = currentFrameTime - System::lastFrameTime;
                System::lastFrameTime = currentFrameTime;

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                if (!userFrameUpdate(System::deltaTime)) {
                    m_gameShouldRun = false;
                }

                if (glfwWindowShouldClose(m_window)) {
                    m_gameShouldRun = false;
                }

                glfwSwapBuffers(m_window);
                glfwPollEvents();
            }
            if (!userDestroy()) {
                throw Exception("Error clearing resources");
            }
        }

        void setClearColor
        ( glm::vec4 color
        ) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void setClearColor
        ( float r
        , float g
        , float b
        , float a
        ) {
            glClearColor(r, g, b, a);
        }

    protected:
        virtual bool userCreate
        (
        ) = 0;

        virtual bool userFrameUpdate
        ( float elapsedTime
        ) = 0;

        virtual bool userDestroy
        (
        ) {
            return true;
        }

        virtual void processInput
        (
        ) {
            if (glfwGetKey(m_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(m_window, true);
            }

            if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
                GameCamera::cam->processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime);
            }
            if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
                GameCamera::cam->processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime);
            }
            if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
                GameCamera::cam->processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime);
            }
            if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
                GameCamera::cam->processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime);
            }
            if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                GameCamera::cam->processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime);
            }
            if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                GameCamera::cam->processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime);
            }

        }

    protected:
        GLFWInitRAII     m_glfwInitializer;
                        
        GLFWwindow      *m_window;
        std::string      m_title;
                        
        eventsQueue      m_eventsQ;
        modelsTable      m_modelsTable;
        gameObjects      m_objects;

        std::atomic_bool m_gameShouldRun;

    public:
    };

} // OGL::E1

#endif // OGL_E1_H
