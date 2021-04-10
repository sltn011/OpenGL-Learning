#include "OGL_E1.hpp"

#include "Utils/System.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace OGL::E1 {

    Engine1Base::Engine1Base(
        int          screenWidth,
        int          screenHeight,
        std::string  title,
        bool         isWindowed,
        int          numSamples
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

    Engine1Base::~Engine1Base(
    ) {
        glfwDestroyWindow(m_window);
    }

    void Engine1Base::start(
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

    void Engine1Base::addModel(
        char const *path,
        size_t modelId
    ) {
        m_modelsTable[modelId] = Model(path);
        m_objectsMaxIDs[modelId] = 0;
    }

    Object &Engine1Base::addNormalObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addNormalObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addNormalObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                xAngle,
                yAngle,
                zAngle,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addNormalObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                xAngle,
                yAngle,
                zAngle,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    std::pair<Object, smartCubemap> &Engine1Base::addMirrorObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    std::pair<Object, smartCubemap> &Engine1Base::addMirrorObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    std::pair<Object, smartCubemap> &Engine1Base::addMirrorObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                xAngle,
                yAngle,
                zAngle,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    std::pair<Object, smartCubemap> &Engine1Base::addMirrorObject(
        size_t modelID,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    std::pair<Object, size_t> &Engine1Base::addInstancedObject(
        size_t modelID,
        size_t numInstances,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    std::pair<Object, size_t> &Engine1Base::addInstancedObject(
        size_t modelID,
        size_t numInstances,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    std::pair<Object, size_t> &Engine1Base::addInstancedObject(
        size_t modelID,
        size_t numInstances,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                xAngle,
                yAngle,
                zAngle,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    std::pair<Object, size_t> &Engine1Base::addInstancedObject(
        size_t modelID,
        size_t numInstances,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID],
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    std::pair<DirectionalLight, smartShadowMap> &Engine1Base::addDirLight(
        glm::vec3 direction,
        glm::vec3 color
    ) {
        return m_scene->addDirLight(
            DirectionalLight{
                direction,
                color
            }
        );
    }

    std::pair<PointLight, smartShadowCubemap> &Engine1Base::addPointLight(
        glm::vec3 position,
        glm::vec3 color,
        float attenuationConst,
        float attenuationLinear,
        float attenuationQuadratic
    ) {
        return m_scene->addPointLight(
            PointLight{
                position,
                color,
                attenuationConst,
                attenuationLinear,
                attenuationQuadratic
            }
        );
    }

    std::pair<SpotLight, smartShadowMap> &Engine1Base::addSpotLight(
        glm::vec3 position,
        glm::vec3 direction,
        glm::vec3 color,
        float cutOffAngle,
        float cutOffOuterAngle,
        float attenuationConst,
        float attenuationLinear,
        float attenuationQuadratic
    ) {
        return m_scene->addSpotLight(
            SpotLight{
                position,
                direction,
                color,
                cutOffAngle,
                cutOffOuterAngle,
                attenuationConst,
                attenuationLinear,
                attenuationQuadratic
            }
        );
    }

    void Engine1Base::setClearColor(
        glm::vec4 color
    ) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Engine1Base::setClearColor(
        float r,
        float g,
        float b,
        float a
    ) {
        glClearColor(r, g, b, a);
    }

    bool Engine1Base::userDestroy(
    ) {
        return true;
    }

    void Engine1Base::processInputPerFrame(
        float speedMult
    ) {
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

    void Engine1Base::windowResizeCallback(
        int newWidth,
        int newHeight
    ) {
        glViewport(0, 0, newWidth, newHeight);
    }

    void Engine1Base::keyCallback(
        int key,
        int scancode,
        int action,
        int mods
    ) {
        if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }
    }

    void Engine1Base::cursorRepositionCallback(
        double xpos,
        double ypos
    ) {

    }

} // OGL::E1
