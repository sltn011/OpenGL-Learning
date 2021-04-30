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
#include "ColoredShapesRenderer.hpp"

#include "LevelSaver.hpp"
#include "LevelLoader.hpp"

#include "GUI/GUIRenderer.hpp"
#include "GUI/CursorPicker.hpp"

#include "Utils/EngineTypes.hpp"
#include "Utils/Events.hpp"
#include "Utils/factory.hpp"
#include "System.hpp"

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
             std::string const &path,
             uint32_t modelId
         );

        Object &addNormalObject( 
            uint32_t modelID, 
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        Object &addNormalObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        Object &addNormalObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        Object &addNormalObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        Object &addTransparentObject( 
            uint32_t modelID, 
            glm::vec3 pos = 
            glm::vec3{ 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f, 
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        Object &addTransparentObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        Object &addTransparentObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        Object &addTransparentObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        std::pair<Object, maybeCubemap> &addMirrorObject(
            uint32_t modelID,
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            float scale = 1.0f,
            float rotationAngle = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        std::pair<Object, maybeCubemap> &addMirrorObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        std::pair<Object, maybeCubemap> &addMirrorObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        std::pair<Object, maybeCubemap> &addMirrorObject(
            uint32_t modelID,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        std::pair<Object, uint32_t> &addInstancedObject(
            uint32_t modelID,
            uint32_t numInstances,
            glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            float scale = 1.0f,
            float rotationAngle = 0.0f,
            glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
        );

        std::pair<Object, uint32_t> &addInstancedObject(
            uint32_t modelID,
            uint32_t numInstances,
            glm::vec3 pos,
            float scale,
            glm::quat quat
        );

        std::pair<Object, uint32_t> &addInstancedObject(
            uint32_t modelID,
            uint32_t numInstances,
            glm::vec3 pos,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        std::pair<Object, uint32_t> &addInstancedObject(
            uint32_t modelID,
            uint32_t numInstances,
            glm::vec3 pos,
            float scale,
            glm::vec3 eulerAngles
        );

        std::pair<DirectionalLight, maybeShadowMap> &addDirLight(
            glm::vec3 direction,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
        );

        std::pair<PointLight, maybeShadowCubemap> &addPointLight(
            glm::vec3 position,
            glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
            float attenuationConst = 1.0f,
            float attenuationLinear = 0.09f,
            float attenuationQuadratic = 0.032f
        );

        std::pair<SpotLight, maybeShadowMap> &addSpotLight(
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

        void setGameOrDebugMode(
            bool isDebugMode
        );

        void loadLevel(
            std::string const &filePath
        );

        void saveLevel(
            std::string const &filePath
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

        virtual void mouseButtonCallback(
            int button,
            int action,
            int mods
        );

        virtual void cursorRepositionCallback(
            double xpos,
            double ypos
        );

        void objectCursorPickerHandler(
        );

     protected:
        GLFWInitRAII     m_glfwInitializer;
                        
        GLFWwindow      *m_window;
        std::string      m_title;

        smartScenePtr                  m_scene;
        maybeNormalRenderer            m_normalRenderer;
        maybeTransparentRenderer       m_transpRenderer;
        maybeSkyboxRenderer            m_skyboxRenderer;
        maybeCubemapRenderer           m_cubemapRenderer;
        maybeMirrorRenderer            m_mirrorRenderer;
        maybeInstancesRenderer         m_instancesRenderer;
        maybeShadowMapRenderer         m_shadowMapRenderer;
        maybeShadowCubemapRenderer     m_shadowCubemapRenderer;
        maybeLightSourcesDebugRenderer m_lightSourcesDebugRenderer;
        maybeColoredShapesRenderer     m_coloredShapesRenderer;

        GUI::maybeGUIRenderer          m_guiRenderer;
                        
        modelsTable       m_modelsTable;
        objectsIDs        m_objectsMaxIDs;

        System            m_system;

        bool              m_gameShouldRun;
        bool              m_showGUI;

        GLFWwindowsizefun  m_windowResizeFunc;
        GLFWkeyfun         m_keyCallbackFunc;
        GLFWmousebuttonfun m_mouseButtonCallbackFunc;
        GLFWcursorposfun   m_cursorReposFunc;

     public:
    };

} // OGL::E1

#endif // OGL_E1_H
