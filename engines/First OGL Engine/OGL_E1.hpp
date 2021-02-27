#ifndef OGL_E1_H
#define OGL_E1_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "Object.hpp"
#include "Shader.hpp"

#include "Scene.hpp"
#include "NormalRenderer.hpp"
#include "TransparentRenderer.hpp"
#include "CubemapRenderer.hpp"
#include "MirrorRenderer.hpp"
#include "InstancesRenderer.hpp"
#include "ShadowMapRenderer.hpp"
#include "ShadowCubemapRenderer.hpp"

#include "Utils/EngineTypes.hpp"
#include "Utils/Events.hpp"
#include "Utils/System.hpp"
#include "Utils/factory.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace OGL::E1 {

    class Engine1Base {
     public:
        Engine1Base( 
            int          screenWidth, 
            int          screenHeight, 
            std::string  title = "Engine1_v.0.2.5", 
            bool         isWindowed = true,
            int          numSamples = 8
        ) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, numSamples);

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
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            glfwSetWindowUserPointer(m_window, this);

            m_windowResizeFunc = [](GLFWwindow *window, int newWidth, int newHeight) {
                static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->windowResizeCallback(newWidth, newHeight);
            };
            glfwSetWindowSizeCallback(m_window, m_windowResizeFunc);

            m_keyCallbackFunc = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->keyCallback(key, scancode, action, mods);
            };
            glfwSetKeyCallback(m_window, m_keyCallbackFunc);

            m_cursorReposFunc = [](GLFWwindow *window, double xpos, double ypos) {
                static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->cursorRepositionCallback(xpos, ypos);
            };
            glfwSetCursorPosCallback(m_window, m_cursorReposFunc);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                throw OGL::Exception("Error loading GLAD!");
            }

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

        }

        virtual ~Engine1Base(
        ) {
            glfwDestroyWindow(m_window);
        }

        void start(
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

        void addModel( 
            char const *path, 
            size_t modelId
        ) {
            m_modelsTable[modelId] = Model(path);
        }

        void addNormalObject( 
            size_t modelID, 
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngleRadians = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        ) {
            m_scene->addNormalObj(
                Object{
                    m_modelsTable[modelID],
                    pos,
                    scale,
                    rotationAngleRadians,
                    rotationAxis
                }
            );
        }

        void addTransparentObject( 
            size_t modelID, 
            glm::vec3 pos = 
            glm::vec3{ 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngleRadians = 0.0f, 
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        ) {
            m_scene->addTransparentObj(
                Object{
                    m_modelsTable[modelID],
                    pos,
                    scale,
                    rotationAngleRadians,
                    rotationAxis
                }
            );
        }

        void addMirrorObject(
            size_t modelID,
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            float scale = 1.0f,
            float rotationAngleRadians = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        ) {
            m_scene->addMirrorObj(
                Object{
                    m_modelsTable[modelID],
                    pos,
                    scale,
                    rotationAngleRadians,
                    rotationAxis
                }
            );
        }

        void addInstancedObject(
            size_t modelID,
            size_t numInstances,
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            float scale = 1.0f,
            float rotationAngleRadians = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        ) {
            m_scene->addInstancedObj(
                Object{
                    m_modelsTable[modelID],
                    pos,
                    scale,
                    rotationAngleRadians,
                    rotationAxis
                },
                numInstances
            );
        }

        void addDirLight( 
            glm::vec3 direction, 
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
        ) {
            m_scene->addDirLight(
                DirectionalLight{
                    direction,
                    color
                }
            );
        }

        void addPointLight(
            glm::vec3 position,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), 
            float attenuationConst = 1.0f, 
            float attenuationLinear = 0.09f, 
            float attenuationQuadratic = 0.032f
        ) {
            m_scene->addPointLight(
                PointLight{
                    position,
                    color,
                    attenuationConst,
                    attenuationLinear,
                    attenuationQuadratic
                }
            );
        }

        void addSpotLight( 
            glm::vec3 position, 
            glm::vec3 direction,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
            float cutOffAngleRadians = glm::radians(8.0f),
            float cutOffOuterAngleRadians = glm::radians(12.0f), 
            float attenuationConst = 1.0f, 
            float attenuationLinear = 0.09f, 
            float attenuationQuadratic = 0.032f
        ) {
            m_scene->addSpotLight(
                SpotLight{
                    position,
                    direction,
                    color,
                    cutOffAngleRadians,
                    cutOffOuterAngleRadians,
                    attenuationConst,
                    attenuationLinear,
                    attenuationQuadratic
                }
            );
        }

        void setClearColor( 
            glm::vec4 color
        ) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void setClearColor( 
            float r = 1.0f, 
            float g = 1.0f, 
            float b = 1.0f, 
            float a = 1.0f
        ) {
            glClearColor(r, g, b, a);
        }

    protected:
        virtual bool userCreate(
        ) = 0;

        virtual bool userFrameUpdate( 
            float elapsedTime
        ) = 0;

        virtual bool userDestroy(
        ) {
            return true;
        }

        virtual void processInputPerFrame( 
            float speedMult = 1.0f
        ) {
            if (glfwGetKey(m_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(m_window, true);
            }

            if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
                m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
                m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
                m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
                m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime * speedMult);
            }
            if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime * speedMult);
            }

            double xMousePos, yMousePos;
            glfwGetCursorPos(m_window, &xMousePos, &yMousePos);
            float xOffset = static_cast<float>(xMousePos) - System::lastMouseXPos;
            float yOffset = System::lastMouseYPos - static_cast<float>(yMousePos);
            System::lastMouseXPos = static_cast<float>(xMousePos);
            System::lastMouseYPos = static_cast<float>(yMousePos);
            m_scene->getCamera()->processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
        }

        virtual void windowResizeCallback(
            int newWidth,
            int newHeight
        ) {
            glViewport(0, 0, newWidth, newHeight);
        }

        virtual void keyCallback(
            int key,
            int scancode,
            int action,
            int mods
        ) {

        }

        virtual void cursorRepositionCallback(
            double xpos,
            double ypos
        ) {

        }

     protected:
        GLFWInitRAII     m_glfwInitializer;
                        
        GLFWwindow      *m_window;
        std::string      m_title;

        smartScenePtr                 m_scene;
        smartNormalRendererPtr        m_normalRenderer;
        smartTransparentRendererPtr   m_transpRenderer;
        smartSkyboxRendererPtr        m_skyboxRenderer;
        smartCubemapRendererPtr       m_cubemapRenderer;
        smartMirrorRendererPtr        m_mirrorRenderer;
        smartInstancesRendererPtr     m_instancesRenderer;
        smartShadowMapRendererPtr     m_shadowMapRenderer;
        smartShadowCubemapRendererPtr m_shadowCubemapRenderer;
                        
        modelsTable       m_modelsTable;

        bool              m_gameShouldRun;

        GLFWwindowsizefun m_windowResizeFunc;
        GLFWkeyfun        m_keyCallbackFunc;
        GLFWcursorposfun  m_cursorReposFunc;


     public:
    };

} // OGL::E1

#endif // OGL_E1_H
