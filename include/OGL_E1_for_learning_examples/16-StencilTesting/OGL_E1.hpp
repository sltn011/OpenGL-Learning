#ifndef OGL_E1_H
#define OGL_E1_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "Object.hpp"
#include "Shader.hpp"

#include "OGL_E1_for_learning_examples/Utils/EngineTypes.hpp"
#include "OGL_E1_for_learning_examples/Utils/Events.hpp"
#include "OGL_E1_for_learning_examples/Utils/16-callbacks.hpp"
#include "OGL_E1_for_learning_examples/Utils/GameCamera.hpp"
#include "OGL_E1_for_learning_examples/Utils/System.hpp"
#include "OGL_E1_for_learning_examples/Utils/factory.hpp"

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
        , std::string  title = "Engine1_v.0.1.2"
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

            glEnable(GL_DEPTH_TEST);
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

        void addModel
        ( char const *path
        , size_t modelId
        ) {
            m_modelsTable[modelId] = OGL::E1::factory<OGL::Model>(path);
        }

        // Returns index of created object in m_objects
        size_t addObject
        ( size_t modelID
        , glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f }
        , float scale = 1.0f
        , float rotationAngleRadians = 0.0f
        , glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        ) {
            m_objects.emplace_back(
                factory<Object>(
                    *m_modelsTable[modelID].get(),
                    pos,
                    scale,
                    rotationAngleRadians,
                    rotationAxis
                )
            );
            return m_objects.size() - 1;
        }

        // Returns index of created dir light in m_dirLights
        size_t addDirLight
        ( glm::vec3 direction
        , glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
        ) {
            m_dirLights.emplace_back(
                factory<DirectionalLight>(
                    direction,
                    color
                )
            );
            return m_dirLights.size() - 1;
        }

        // Returns index of created point light in m_pointLights
        size_t addPointLight
        ( glm::vec3 position
        , glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
        , float attenuationConst = 1.0f
        , float attenuationLinear = 0.09f
        , float attenuationQuadratic = 0.032f
        ) {
            m_pointLights.emplace_back(
                factory<PointLight>(
                    position,
                    color,
                    attenuationConst,
                    attenuationLinear,
                    attenuationQuadratic
                )
            );
            return m_pointLights.size() - 1;
        }

        // Returns index of created spot light in m_spotLights
        size_t addSpotLight
        ( glm::vec3 position
        , glm::vec3 direction
        , glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
        , float cutOffAngleRadians = glm::radians(8.0f)
        , float cutOffOuterAngleRadians = glm::radians(12.0f)
        , float attenuationConst = 1.0f
        , float attenuationLinear = 0.09f
        , float attenuationQuadratic = 0.032f
        ) {
            m_spotLights.emplace_back(
                factory<SpotLight>(
                    position,
                    direction,
                    color,
                    cutOffAngleRadians,
                    cutOffOuterAngleRadians,
                    attenuationConst,
                    attenuationLinear,
                    attenuationQuadratic
                )
            );
            return m_spotLights.size() - 1;
        }

        void setClearColor
        ( glm::vec4 color
        ) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void setClearColor
        ( float r = 1.0f
        , float g = 1.0f
        , float b = 1.0f
        , float a = 1.0f
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
        ( float speedMult = 1.0f
        ) {
            if (glfwGetKey(m_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(m_window, true);
            }

            if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
                GameCamera::inst->processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
                GameCamera::inst->processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
                GameCamera::inst->processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
                GameCamera::inst->processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                GameCamera::inst->processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                GameCamera::inst->processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime * speedMult);
            }

        }

    protected:
        GLFWInitRAII     m_glfwInitializer;
                        
        GLFWwindow      *m_window;
        std::string      m_title;
                        
        eventsQueue      m_eventsQ;
        modelsTable      m_modelsTable;
        gameObjects      m_objects;
        shadersPack      m_shaders;
        dirLights        m_dirLights;
        pointLights      m_pointLights;
        spotLights       m_spotLights;

        std::atomic_bool m_gameShouldRun;

    public:
    };

} // OGL::E1

#endif // OGL_E1_H
