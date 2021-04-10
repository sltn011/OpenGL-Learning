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
#include "LightSourcesDebugRenderer.hpp"

#include "Utils/EngineTypes.hpp"
#include "Utils/Events.hpp"
#include "Utils/factory.hpp"

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
             std::string  title = "Engine1_v.0.2.7.2",
             bool         isWindowed = true,
             int          numSamples = 8
         );

         virtual ~Engine1Base(
         );

         void start(
         );

         void addModel(
             char const *path,
             size_t modelId
         );

        Object &addNormalObject( 
            size_t modelID, 
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        Object &addNormalObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        Object &addNormalObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        Object &addNormalObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        Object &addTransparentObject( 
            size_t modelID, 
            glm::vec3 pos = 
            glm::vec3{ 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f, 
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        Object &addTransparentObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        Object &addTransparentObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        Object &addTransparentObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        std::pair<Object, smartCubemap> &addMirrorObject(
            size_t modelID,
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            float scale = 1.0f,
            float rotationAngle = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        std::pair<Object, smartCubemap> &addMirrorObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        std::pair<Object, smartCubemap> &addMirrorObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        std::pair<Object, smartCubemap> &addMirrorObject(
            size_t modelID,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        std::pair<Object, size_t> &addInstancedObject(
            size_t modelID,
            size_t numInstances,
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            float scale = 1.0f,
            float rotationAngle = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        std::pair<Object, size_t> &addInstancedObject(
            size_t modelID,
            size_t numInstances,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        std::pair<Object, size_t> &addInstancedObject(
            size_t modelID,
            size_t numInstances,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        std::pair<Object, size_t> &addInstancedObject(
            size_t modelID,
            size_t numInstances,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        std::pair<DirectionalLight, smartShadowMap> &addDirLight(
            glm::vec3 direction,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
        );

        std::pair<PointLight, smartShadowCubemap> &addPointLight(
            glm::vec3 position,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
            float attenuationConst = 1.0f,
            float attenuationLinear = 0.09f,
            float attenuationQuadratic = 0.032f
        );

        std::pair<SpotLight, smartShadowMap> &addSpotLight(
            glm::vec3 position,
            glm::vec3 direction,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
            float cutOffAngle = 8.0f,
            float cutOffOuterAngle = 12.0f,
            float attenuationConst = 1.0f,
            float attenuationLinear = 0.09f,
            float attenuationQuadratic = 0.032f
        );

        void setClearColor(
            glm::vec4 color
        );

        void setClearColor(
            float r = 1.0f,
            float g = 1.0f,
            float b = 1.0f,
            float a = 1.0f
        );

     protected:
        virtual bool userCreate(
        ) = 0;

        virtual bool userFrameUpdate( 
            float elapsedTime
        ) = 0;

        virtual bool userDestroy(
        );

        virtual void processInputPerFrame(
            float speedMult = 1.0f
        );

        virtual void windowResizeCallback(
            int newWidth,
            int newHeight
        );

        virtual void keyCallback(
            int key,
            int scancode,
            int action,
            int mods
        );

        virtual void cursorRepositionCallback(
            double xpos,
            double ypos
        );

     protected:
        GLFWInitRAII     m_glfwInitializer;
                        
        GLFWwindow      *m_window;
        std::string      m_title;

        smartScenePtr                     m_scene;
        smartNormalRendererPtr            m_normalRenderer;
        smartTransparentRendererPtr       m_transpRenderer;
        smartSkyboxRendererPtr            m_skyboxRenderer;
        smartCubemapRendererPtr           m_cubemapRenderer;
        smartMirrorRendererPtr            m_mirrorRenderer;
        smartInstancesRendererPtr         m_instancesRenderer;
        smartShadowMapRendererPtr         m_shadowMapRenderer;
        smartShadowCubemapRendererPtr     m_shadowCubemapRenderer;
        smartLightSourcesDebugRendererPtr m_lightSourcesDebugRenderer;
                        
        modelsTable       m_modelsTable;
        objectsIDs        m_objectsMaxIDs;

        bool              m_gameShouldRun;

        GLFWwindowsizefun m_windowResizeFunc;
        GLFWkeyfun        m_keyCallbackFunc;
        GLFWcursorposfun  m_cursorReposFunc;

     public:
    };

} // OGL::E1

#endif // OGL_E1_H
