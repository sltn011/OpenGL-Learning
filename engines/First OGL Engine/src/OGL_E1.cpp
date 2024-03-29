#include "OGL_E1.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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
        m_windowSamples = numSamples;

        GLFWmonitor *monitor = isWindowed ? nullptr : glfwGetPrimaryMonitor();

        GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), monitor, nullptr);
        if (!window) {
            throw OGL::Exception("Error creating window!");
        }
        m_window = window;

        glfwMakeContextCurrent(m_window);

        glfwSetWindowSizeLimits(m_window, GLFW_DONT_CARE, GLFW_DONT_CARE, screenWidth, screenHeight);
        glfwSetWindowSize(m_window, screenWidth, screenHeight);

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetWindowUserPointer(m_window, this);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw OGL::Exception("Error loading GLAD!");
        }

        m_windowResizeFunc = [](GLFWwindow *window, int newWidth, int newHeight) {
            static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->windowResizeCallback(newWidth, newHeight);
        };
        glfwSetWindowSizeCallback(m_window, m_windowResizeFunc);

        m_keyCallbackFunc = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->keyCallback(key, scancode, action, mods);
        };
        glfwSetKeyCallback(m_window, m_keyCallbackFunc);

        m_mouseButtonCallbackFunc = [](GLFWwindow *window, int button, int action, int mods) {
            static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(button, action, mods);
        };
        glfwSetMouseButtonCallback(m_window, m_mouseButtonCallbackFunc);

        m_cursorReposFunc = [](GLFWwindow *window, double xpos, double ypos) {
            static_cast<Engine1Base*>(glfwGetWindowUserPointer(window))->cursorRepositionCallback(xpos, ypos);
        };
        glfwSetCursorPosCallback(m_window, m_cursorReposFunc);

        m_system.lastMouseXPos = screenWidth / 2.0f;
        m_system.lastMouseYPos = screenHeight / 2.0f;

        m_title = title;
        m_showGUI = false;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        UtilsLibrary::m_engine = this;

        if (m_postprocessingData.bEnableSSAO) {
            toggleSSAO(true);
        }
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
        m_system.lastFrameTime = (float)glfwGetTime();
        while (m_gameShouldRun) {
            float currentFrameTime = (float)glfwGetTime();
            m_system.deltaTime = currentFrameTime - m_system.lastFrameTime;
            m_system.lastFrameTime = currentFrameTime;

            if (m_gBuffer) {
                deferredRenderPass();

                if (m_transpRenderer) {
                    glEnable(GL_BLEND);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    m_transpRenderer->render(*m_scene, m_scene->getCamera().get());
                    glDisable(GL_BLEND);
                }
            }
            else {
                forwardRenderPass();
            }

            if (m_showGUI && m_guiRenderer) {
                m_guiRenderer->render();
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
        std::string const &path,
        uint32_t modelID
    ) {
        m_modelsTable.emplace(modelID, std::make_unique<Model>(path));
        m_objectsMaxIDs[modelID] = 0;
    }

    void Engine1Base::addModel(
        Model &&model,
        uint32_t modelID
    ) {
        m_modelsTable.emplace(modelID, std::make_unique<Model>(std::move(model)));
        m_objectsMaxIDs[modelID] = 0;
    }

    Object &Engine1Base::addNormalObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addNormalObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addNormalObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID].get(),
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
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addNormalObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    Object &Engine1Base::addTransparentObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID].get(),
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
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addTransparentObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    pMirrorCubemap &Engine1Base::addMirrorObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    pMirrorCubemap &Engine1Base::addMirrorObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    pMirrorCubemap &Engine1Base::addMirrorObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                xAngle,
                yAngle,
                zAngle,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    pMirrorCubemap &Engine1Base::addMirrorObject(
        uint32_t modelID,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addMirrorObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            }
        );
    }

    pObjInstancesNum &Engine1Base::addInstancedObject(
        uint32_t modelID,
        uint32_t numInstances,
        glm::vec3 pos,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                rotationAngle,
                rotationAxis,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    pObjInstancesNum &Engine1Base::addInstancedObject(
        uint32_t modelID,
        uint32_t numInstances,
        glm::vec3 pos,
        float scale,
        glm::quat quat
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                quat,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    pObjInstancesNum &Engine1Base::addInstancedObject(
        uint32_t modelID,
        uint32_t numInstances,
        glm::vec3 pos,
        float scale,
        float xAngle,
        float yAngle,
        float zAngle
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID].get(),
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

    pObjInstancesNum &Engine1Base::addInstancedObject(
        uint32_t modelID,
        uint32_t numInstances,
        glm::vec3 pos,
        float scale,
        glm::vec3 eulerAngles
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addInstancedObj(
            Object{
                m_modelsTable[modelID].get(),
                pos,
                scale,
                eulerAngles,
                m_objectsMaxIDs[modelID]++
            },
            numInstances
        );
    }

    pDirLightShadow &Engine1Base::addDirLight(
        glm::vec3 direction,
        glm::vec3 color
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

        return m_scene->addDirLight(
            DirectionalLight{
                direction,
                color
            }
        );
    }

    pPointLightShadows &Engine1Base::addPointLight(
        glm::vec3 position,
        glm::vec3 color,
        float attenuationConst,
        float attenuationLinear,
        float attenuationQuadratic
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

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

    pSpotLightShadows &Engine1Base::addSpotLight(
        glm::vec3 position,
        glm::vec3 direction,
        glm::vec3 color,
        float cutOffAngle,
        float cutOffOuterAngle,
        float attenuationConst,
        float attenuationLinear,
        float attenuationQuadratic
    ) {
        if (!m_scene) {
            throw Exception("Scene not created!");
        }

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

    void Engine1Base::setGameOrDebugMode(
        bool isDebugMode
    ) {
        m_showGUI = isDebugMode;
        if (m_showGUI) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(m_window, m_system.lastMouseXPos, m_system.lastMouseYPos);
        }
    }

    void Engine1Base::loadLevel(
        std::string const &filePath
    ) {
        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);

        OGL::E1::LevelLoader loader;
        loader.load(
            screenWidth,
            screenHeight,
            filePath,
            m_modelsTable,
            m_scene
        );
    }

    void Engine1Base::saveLevel(
        std::string const &filePath
    ) {
        if (!m_scene) {
            return;
        }

        OGL::E1::LevelSaver saver;
        saver.save(
            filePath,
            m_modelsTable,
            *m_scene
        );
    }

    void Engine1Base::rebuildDirLightsShadows(
    ) {
        if (!m_scene || !m_shadowMapRenderer) {
            return;
        }

        glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);
        for (size_t i = 0; i < m_scene->getDirLights().size(); ++i) {
            auto &[dirLight, shadowMap] = m_scene->getDirLights()[i];
            OGL::CameraShadowMap cam{ dirLight, m_scene->getNormalObjs().front().getPosition(), 2.5f, 0.1f, 6.0f };
            shadowMap = m_shadowMapRenderer->render(
                *m_scene,
                cam,
                static_cast<GLenum>(GL_TEXTURE0 + m_shadowMapDirLightFirstTextureID + i),
                m_shadowMapSize
            );
        }

        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
    }

    void Engine1Base::rebuildPointLightsShadows(
    ) {
        if (!m_scene || !m_shadowCubemapRenderer) {
            return;
        }

        glViewport(0, 0, m_shadowCubemapSize, m_shadowCubemapSize);
        for (size_t i = 0; i < m_scene->getPointLights().size(); ++i) {
            auto &[pointLight, shadowCubemap] = m_scene->getPointLights()[i];
            OGL::CameraShadowCubemap cam(pointLight, 0.01f, 3.5f);
            shadowCubemap = m_shadowCubemapRenderer->render(
                *m_scene,
                cam,
                static_cast<GLenum>(GL_TEXTURE0 + m_shadowCubemapFirstTextureID + i),
                m_shadowCubemapSize
            );
        }

        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
    }

    void Engine1Base::rebuildSpotLightsShadows(
    ) {
        if (!m_scene || !m_shadowMapRenderer) {
            return;
        }

        glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);
        for (size_t i = 0; i < m_scene->getSpotLights().size(); ++i) {
            auto &[spotLight, shadowMap] = m_scene->getSpotLights()[i];
            OGL::CameraShadowMap cam{ spotLight, 0.1f, 6.0f };
            shadowMap = m_shadowMapRenderer->render(
                *m_scene,
                cam,
                static_cast<GLenum>(GL_TEXTURE0 + m_shadowMapSpotLightFirstTextureID + i),
                m_shadowMapSize
            );
        }
        
        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
    }

    void Engine1Base::rebuildShadows(
    ) {
        rebuildDirLightsShadows();
        rebuildPointLightsShadows();
        rebuildSpotLightsShadows();
    }

    void Engine1Base::rebuildReflections(
    ) {
        if (!m_scene || !m_cubemapRenderer) {
            return;
        }

        glViewport(0, 0, m_mirrorCubemapSize, m_mirrorCubemapSize);
        for (auto &p : m_scene->getMirrorObjs()) {
            p.second = m_cubemapRenderer->render(
                *m_scene,
                m_mirrorCubemapSize,
                static_cast<GLenum>(GL_TEXTURE0 + m_mirrorCubemapFirstTextureID),
                p.first.getPosition(),
                m_normalRenderer,
                m_skyboxRenderer,
                m_transpRenderer,
                m_instancesRenderer
            );
        }

        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
    }

    void Engine1Base::initPostprocessing(
        Shader &&postprocessingShader
    ) {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);

        m_renderFramebuffer = FrameBufferObject{ 0, 1 };
        m_renderFramebuffer->bind(GL_FRAMEBUFFER);

        ColorBufferObject renderColorBuffer;
        RenderBufferObject renderDepthBuffer;

        if (m_windowSamples != 1) {
            renderColorBuffer.allocateStorageMultisample(width, height, m_windowSamples, GL_RGBA16F);
            renderDepthBuffer.allocateStorageMultisample(width, height, m_windowSamples, GL_DEPTH24_STENCIL8);
            m_renderFramebuffer->attachColorBufferMultisample(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(renderColorBuffer));
            m_renderFramebuffer->attachRenderBufferMultisample(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, std::move(renderDepthBuffer));
        }
        else {
            renderColorBuffer.allocateStorage(width, height, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
            renderDepthBuffer.allocateStorage(width, height, GL_DEPTH24_STENCIL8);
            m_renderFramebuffer->attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(renderColorBuffer));
            m_renderFramebuffer->attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, std::move(renderDepthBuffer));
        }

        if (!m_renderFramebuffer->isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception{ "Error creating render framebuffer!" };
        }



        m_postprocessingFramebuffer = FrameBufferObject{ 0, 1 };
        m_postprocessingFramebuffer->bind(GL_FRAMEBUFFER);

        ColorBufferObject postprocessingColorBuffer;
        RenderBufferObject postprocessingDepthBuffer;

        postprocessingColorBuffer.allocateStorage(width, height, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        postprocessingDepthBuffer.allocateStorage(width, height, GL_DEPTH24_STENCIL8);
        m_postprocessingFramebuffer->attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(postprocessingColorBuffer));
        m_postprocessingFramebuffer->attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, std::move(postprocessingDepthBuffer));
        
        if (!m_postprocessingFramebuffer->isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception{ "Error creating postprocessing framebuffer!" };
        }


        FrameBufferObject::unbind(GL_FRAMEBUFFER);

        m_postprocessingShader = std::move(postprocessingShader);
    }

    void Engine1Base::drawPostprocessingQuad(
    ) {
        if (!m_renderFramebuffer || !m_postprocessingFramebuffer || !m_postprocessingShader) {
            return;
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_postprocessingShader->use();
        loadPostprocessingShader();
        m_postprocessingFramebuffer->drawQuad(GL_COLOR_ATTACHMENT0);
    }

    void Engine1Base::loadPostprocessingShader(
    ) {
        if (!m_postprocessingShader) {
            return;
        }

        m_postprocessingShader->setUniformInt("fboTexture", 0);
        m_postprocessingShader->setUniformBool("bEnableHDR", m_postprocessingData.bEnableHDR);
        m_postprocessingShader->setUniformFloat("HDRExposure", m_postprocessingData.HDRExposure);
        m_postprocessingShader->setUniformBool("bEnableBloom", m_postprocessingData.bEnableBloom);
    }

    PostprocessingData Engine1Base::getPostprocessingData(
    ) const {
        return m_postprocessingData;
    }

    void Engine1Base::togglePostprocessing(
        bool bEnabled
    ) {
        m_postprocessingData.bEnablePostprocessing = bEnabled;
    }

    void Engine1Base::toggleHDR(
        bool bEnabled
    ) {
        m_postprocessingData.bEnableHDR = bEnabled;
    }

    void Engine1Base::setHDRExposure(
        float exposure
    ) {
        m_postprocessingData.HDRExposure = exposure;
    }

    void Engine1Base::toggleBloom(
        bool bEnabled
    ) {
        m_postprocessingData.bEnableBloom = bEnabled;
    }

    void Engine1Base::initBlur(
        Shader &&horizontalBlurShader,
        Shader &&verticalBlurShader
    ) {
        m_horizontalBlurShader = std::move(horizontalBlurShader);
        m_verticalBlurShader = std::move(verticalBlurShader);

        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);

        ColorBufferObject cbo;
        cbo.allocateStorage(screenWidth, screenHeight, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);

        m_blurIntermediateBuffer = FrameBufferObject{};
        m_blurIntermediateBuffer->bind(GL_FRAMEBUFFER);
        m_blurIntermediateBuffer->attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(cbo));
        if (!m_blurIntermediateBuffer->isComplete(GL_FRAMEBUFFER)) {
            throw Exception("Error creating blur framebuffer!");
        }
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void Engine1Base::blurImage(
        FrameBufferObject &fbo,
        GLenum blurredColorAttachment
    ) {
        if (!m_horizontalBlurShader || !m_verticalBlurShader) {
            return;
        }

        int imageWidth, imageHeight;
        glfwGetWindowSize(m_window, &imageWidth, &imageHeight);

        m_horizontalBlurShader->use();
        m_horizontalBlurShader->setUniformInt("fboTexture", blurredColorAttachment - GL_COLOR_ATTACHMENT0);
        m_horizontalBlurShader->setUniformInt("imageWidth", imageWidth);
        m_blurIntermediateBuffer->bind(GL_FRAMEBUFFER);
        fbo.drawQuad(blurredColorAttachment);

        m_verticalBlurShader->use();
        m_verticalBlurShader->setUniformInt("fboTexture", 0);
        m_verticalBlurShader->setUniformInt("imageHeight", imageHeight);
        fbo.bind(GL_FRAMEBUFFER);
        m_blurIntermediateBuffer->drawQuad(GL_COLOR_ATTACHMENT0);

        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void Engine1Base::toggleSSAO(
        bool bEnabled
    ) {
        m_postprocessingData.bEnableSSAO = bEnabled;

        if (m_postprocessingData.bEnableSSAO) {
            int width = 0, height = 0;
            glfwGetWindowSize(m_window, &width, &height);

            m_SSAOBuffer = SSAO{width, height};
        }
        else {
            m_SSAOBuffer = std::nullopt;
        }
    }

    void Engine1Base::fillSSAOTexture(
    ) {
        if (!m_postprocessingData.bEnableSSAO || !m_SSAOBuffer || !m_SSAOShader || !m_gBuffer || !m_scene) {
            return;
        }

        m_SSAOShader->use();
        loadGBufferReadShaderData(*m_SSAOShader);
        m_SSAOShader->setUniformMatrix4("projection", m_scene->getCamera()->getProjectionMatrix());
        m_SSAOBuffer->bindAsShaderOutput(
            *m_SSAOShader,
            GL_TEXTURE0 + GBuffer::Buffer::MAX,
            GL_TEXTURE0 + GBuffer::Buffer::MAX + 1
        );
        m_gBuffer->drawQuad(*m_SSAOShader);
        FrameBufferObject::unbind(GL_FRAMEBUFFER);

        blurImage(m_SSAOBuffer->getBuffer(), GL_COLOR_ATTACHMENT0);
    }

    void Engine1Base::forwardRenderPass(
    ) {
        if (m_postprocessingData.bEnablePostprocessing && m_postprocessingFramebuffer && m_renderFramebuffer && m_postprocessingShader) {
            m_renderFramebuffer->bind(GL_FRAMEBUFFER);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (!userFrameUpdate(m_system.deltaTime)) {
            m_gameShouldRun = false;
        }

        if (m_postprocessingData.bEnablePostprocessing && m_postprocessingFramebuffer && m_renderFramebuffer && m_postprocessingShader) {

            int width, height;
            glfwGetWindowSize(m_window, &width, &height);

            if (m_postprocessingData.bEnableBloom && m_bloom) {
                m_bloom->drawToResultFrameBuffer(*m_renderFramebuffer);
                m_bloom->bindResultFrameBuffer(GL_READ_FRAMEBUFFER);
            }
            else {
                m_renderFramebuffer->bind(GL_READ_FRAMEBUFFER);
            }

            m_postprocessingFramebuffer->bind(GL_DRAW_FRAMEBUFFER);
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            drawPostprocessingQuad();
        }
    }

    void Engine1Base::deferredRenderPass(
    ) {
        if (!m_gBuffer || !m_gBufferWriteShader || !m_gBufferReadShader) {
            m_gameShouldRun = false;
        }

        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);

        m_gBufferWriteShader->use();
        loadGBufferWriteShaderData(*m_gBufferWriteShader);

        m_gBuffer->bindAsRenderTarget();

        glDisable(GL_BLEND);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (!userFrameUpdate(m_system.deltaTime)) {
            m_gameShouldRun = false;
        }

        if (!m_postprocessingData.bEnablePostprocessing || !m_postprocessingFramebuffer || !m_renderFramebuffer || !m_postprocessingShader) {
            m_gBufferReadShader->use();
            loadGBufferReadShaderData(*m_gBufferReadShader);

            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            m_gBuffer->drawQuad(*m_gBufferReadShader);
        }
        else {
            if (m_postprocessingData.bEnableSSAO && m_SSAOBuffer && m_SSAOShader) {
                fillSSAOTexture();
            }

            m_gBufferReadShader->use();
            loadGBufferReadShaderData(*m_gBufferReadShader);

            if (m_postprocessingData.bEnableSSAO && m_SSAOBuffer) {
                m_SSAOBuffer->bindAsShaderInput(*m_gBufferReadShader, GL_TEXTURE0 + GBuffer::Buffer::MAX);
            }

            m_renderFramebuffer->bind(GL_FRAMEBUFFER);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            m_gBuffer->drawQuad(*m_gBufferReadShader);

            int width, height;
            glfwGetWindowSize(m_window, &width, &height);

            if (m_postprocessingData.bEnableBloom && m_bloom) {
                m_bloom->drawToResultFrameBuffer(*m_renderFramebuffer);
                m_bloom->bindResultFrameBuffer(GL_READ_FRAMEBUFFER);
            }
            else {
                m_renderFramebuffer->bind(GL_READ_FRAMEBUFFER);
            }

            m_postprocessingFramebuffer->bind(GL_DRAW_FRAMEBUFFER);
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            drawPostprocessingQuad();
        }
    }

    void Engine1Base::loadGBufferWriteShaderData(
        Shader &writeShader
    ) {
        if (!m_scene) {
            return;
        }

        BasicCamera *camera = m_scene->getCamera().get();

        writeShader.setUniformMatrix4("view", camera->getViewMatrix());
        writeShader.setUniformMatrix4("projection", camera->getProjectionMatrix());
        writeShader.setUniformVec3("viewerPos", camera->getPos());
    }

    void Engine1Base::loadGBufferReadShaderData(
        Shader &readShader
    ) {
        if (!m_scene) {
            return;
        }

        readShader.setUniformBool("bEnableSSAO", m_postprocessingData.bEnableSSAO);

        BasicCamera *camera = m_scene->getCamera().get();

        readShader.setUniformMatrix4("view", camera->getViewMatrix());
        readShader.setUniformMatrix4("projection", camera->getProjectionMatrix());
        readShader.setUniformVec3("viewerPos", camera->getPos());

        dirLights &sceneDirLights = m_scene->getDirLights();
        readShader.setUniformInt("numDirLights", static_cast<int>(sceneDirLights.size()));
        for (size_t i = 0; i < sceneDirLights.size(); ++i) {
            auto &[light, pShadowMap] = sceneDirLights[i];
            light.loadInShader(readShader, static_cast<int>(i));
            if (!pShadowMap) {
                continue;
            }
            pShadowMap->bindTexture();
            readShader.setUniformMatrix4("dirLightProjView[" + std::to_string(i) + "]", pShadowMap->lightProjView());
            readShader.setUniformInt("dirLightShadowMap[" + std::to_string(i) + "]", pShadowMap->textureUnit() - GL_TEXTURE0);
        }

        pointLights &scenePointLights = m_scene->getPointLights();
        readShader.setUniformInt("numPointLights", static_cast<int>(scenePointLights.size()));
        for (size_t i = 0; i < scenePointLights.size(); ++i) {
            auto &[light, pShadowCubemap] = scenePointLights[i];
            light.loadInShader(readShader, static_cast<int>(i));
            if (!pShadowCubemap) {
                continue;
            }
            pShadowCubemap->bindTexture();
            readShader.setUniformInt("pointLightShadowMap[" + std::to_string(i) + "]", pShadowCubemap->textureUnit() - GL_TEXTURE0);
            readShader.setUniformFloat("pointLightShadowMapFarPlane[" + std::to_string(i) + "]", pShadowCubemap->farPlane());
        }

        spotLights &sceneSpotLights = m_scene->getSpotLights();
        readShader.setUniformInt("numSpotLights", static_cast<int>(sceneSpotLights.size()));
        for (size_t i = 0; i < sceneSpotLights.size(); ++i) {
            auto &[light, pShadowMap] = sceneSpotLights[i];
            light.loadInShader(readShader, static_cast<int>(i));
            if (!pShadowMap) {
                continue;
            }
            pShadowMap->bindTexture();
            readShader.setUniformMatrix4("spotLightProjView[" + std::to_string(i) + "]", pShadowMap->lightProjView());
            readShader.setUniformInt("spotLightShadowMap[" + std::to_string(i) + "]", pShadowMap->textureUnit() - GL_TEXTURE0);
        }
    }

    bool Engine1Base::userDestroy(
    ) {
        return true;
    }

    void Engine1Base::processInputPerFrame(
        float speedMult
    ) {
        if (m_showGUI) {
            return;
        }

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Forward, m_system.deltaTime * speedMult);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Backward, m_system.deltaTime * speedMult);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Left, m_system.deltaTime * speedMult);
        }
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Right, m_system.deltaTime * speedMult);
        }
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Up, m_system.deltaTime * speedMult);
        }
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            m_scene->getCamera()->processMoveInput(OGL::CameraMovementDirection::Down, m_system.deltaTime * speedMult);
        }

        double xMousePos, yMousePos;
        glfwGetCursorPos(m_window, &xMousePos, &yMousePos);
        float xOffset = static_cast<float>(xMousePos) - m_system.lastMouseXPos;
        float yOffset = m_system.lastMouseYPos - static_cast<float>(yMousePos);
        m_system.lastMouseXPos = static_cast<float>(xMousePos);
        m_system.lastMouseYPos = static_cast<float>(yMousePos);
        m_scene->getCamera()->processRotateInput(xOffset, yOffset, m_system.mouseSensitivity, true);
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
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }
        if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_PRESS) {
            setGameOrDebugMode(!m_showGUI);
        }
    }

    void Engine1Base::mouseButtonCallback(
        int button, 
        int action, 
        int mods
    ) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            if (m_showGUI && m_coloredShapesRenderer) {
                objectCursorPickerHandler();
            }
        }
    }

    void Engine1Base::cursorRepositionCallback(
        double xpos,
        double ypos
    ) {

    }

    void Engine1Base::objectCursorPickerHandler(
    ) {
        if (!m_coloredShapesRenderer) {
            return;
        }

        int screenWidth, screenHeight;
        glfwGetWindowSize(m_window, &screenWidth, &screenHeight);

        auto[fbo, map] = m_coloredShapesRenderer->render(*m_scene, m_scene->getCamera().get(), screenWidth, screenHeight);
        Object *selectedObj = GUI::CursorPicker{}.getSelected(fbo, map, m_window);

        if (m_guiRenderer) {
            auto &windowsMap = m_guiRenderer->getWindows();

            auto maybeWindow = windowsMap.find(GUI::WindowsTypes::ObjectTransform);
            if (maybeWindow != windowsMap.end()) {
                if (maybeWindow->second->m_enabled) {
                    GUI::ObjectTransformWindow* transformWindow = dynamic_cast<GUI::ObjectTransformWindow*>(maybeWindow->second.get());
                    transformWindow->setObject(selectedObj);
                }
            }
        }
    }

} // OGL::E1
