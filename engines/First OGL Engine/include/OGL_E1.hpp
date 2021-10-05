/**
* @file OGL_E1.hpp
*/

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

#include "PostprocessingData.hpp"

#include "LevelSaver.hpp"
#include "LevelLoader.hpp"

#include "GUI/GUIRenderer.hpp"
#include "GUI/CursorPicker.hpp"

#include "Utils/EngineTypes.hpp"
#include "Utils/Events.hpp"
#include "Utils/factory.hpp"
#include "Utils/UtilsLibrary.hpp"
#include "System.hpp"

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace OGL::E1 {

    /**
     * @brief Abstract class which implements basic functions to render 3D scene and handle user input.
     *
     * To create a game, inherit this class and provide implementation to two abstract methods: userCreate and userFrameUpdate\n
     * @ref userCreate - resources initialization\n
     * @ref userFrameUpdate - procesing user input and updating scene
    */
    class Engine1Base {
     protected:
        /// Pointer to glfw window structure used to output rendered scene
        GLFWwindow      *m_window;

        /// Title of window used to output rendered scene
        std::string      m_title;



        /// Scene class which contains camera, objects and light sources, or nullptr if not created - needs to be created
        smartScenePtr                  m_scene;

        /// Renderer for Scene's normal Objects, or std::nullopt if not created yet - not mandatory
        maybeNormalRenderer            m_normalRenderer;

        /// Renderer for Scene's transparent Objects, or std::nullopt if not created yet - not mandatory
        maybeTransparentRenderer       m_transpRenderer;

        /// Renderer for Scene's Skybox, or std::nullopt if not created yet - not mandatory
        maybeSkyboxRenderer            m_skyboxRenderer;

        /// Renderer for Scene's mirror Objects, or std::nullopt if not created yet - not mandatory
        maybeMirrorRenderer            m_mirrorRenderer;

        /// Renderer for Scene's instanced Objects, or std::nullopt if not created yet - not mandatory
        maybeInstancesRenderer         m_instancesRenderer;

        /// Renderer for Cubemaps, or std::nullopt if not created yet - not mandatory
        maybeCubemapRenderer           m_cubemapRenderer;
        
        /// Renderer for ShadowMaps, or std::nullopt if not created yet - not mandatory
        maybeShadowMapRenderer         m_shadowMapRenderer;

        /// Renderer for ShadowCubemaps, or std::nullopt if not created yet - not mandatory
        maybeShadowCubemapRenderer     m_shadowCubemapRenderer;

        /// Renderer for PointLights and SpotLights as colored cubes, or std::nullopt if not created yet - not mandatory
        maybeLightSourcesDebugRenderer m_lightSourcesDebugRenderer;

        /// Renderer of all Scene's normal, transparent and mirror Objects as their shapes with unique colors, or std::nullopt if not created yet - not mandatory
        maybeColoredShapesRenderer     m_coloredShapesRenderer;

        /// Renderer of GUI, or std::nullopt if not created yet - not mandatory
        GUI::maybeGUIRenderer          m_guiRenderer;

        /// Shader used for postprocessing
        std::optional<Shader> m_postprocessingShader;

        /// Framebuffers used for postprocessing
        std::optional<FrameBufferObject> m_renderFramebuffer;
        std::optional<FrameBufferObject> m_postprocessingFramebuffer;


        /// Hashmap of Key: Model's ID, Value: Owning pointer to Model object - stores Models used in Scene
        modelsTable       m_modelsTable;

        /// Hashmap of Key: Model's ID, Value: Maximum ID of Object with this Model
        objectsIDs        m_objectsMaxIDs;



        /// System data used in Engine
        System            m_system;



        /// Flag to control Engine's game loop - if set to true loop will run, else - will end game loop and deallocate resources
        bool              m_gameShouldRun;

        /// Flag to render Engine's GUI - if set to true GUI will be rendered over the scene, else - does nothing
        bool              m_showGUI;



        /// GLFW initialization
        GLFWInitRAII       m_glfwInitializer;



        /// GLFW window resize callback function which calls Engine1Base's or override of windowResizeCallback @ref windowResizeCallback
        GLFWwindowsizefun  m_windowResizeFunc;

        /// GLFW keyboard input callback function which calls Engine1Base's or override of keyCallback @ref keyCallback
        GLFWkeyfun         m_keyCallbackFunc;

        /// GLFW mouse button input callback function which calls Engine1Base's or override of mouseButtonCallback @ref mouseButtonCallback
        GLFWmousebuttonfun m_mouseButtonCallbackFunc;

        /// GLFW cursor reposition callback function which calls Engine1Base's or override of cursorRepositionCallback @ref cursorRepositionCallback
        GLFWcursorposfun   m_cursorReposFunc;

        PostprocessingData m_postprocessingData;

        int32_t m_windowSamples;

    public:

        /// ShadowMap data
        int32_t m_shadowMapSize = 1024;

        /// ShadowCubemap size
        int32_t m_shadowCubemapSize = 1024;

        /// ShadowMaps texture ID's
        int32_t m_shadowMapDirLightFirstTextureID = 10;
        int32_t m_shadowCubemapFirstTextureID = m_shadowMapDirLightFirstTextureID + 4;
        int32_t m_shadowMapSpotLightFirstTextureID = m_shadowCubemapFirstTextureID + 4;

        /// Mirror Cubemap size
        int32_t m_mirrorCubemapSize = 256;
        int32_t m_mirrorCubemapFirstTextureID = 23;

     public:
         /**
          * @brief Engine1Base constructor
          * @param screenWidth Screen width
          * @param screenHeight Screen height
          * @param title Window title
          * @param isWindowed true if windowed mode, false if should be fullscreen
          * @param numSamples Number of samples in screen buffer - bigger number makes image smoother, but can lower performance
         */
         Engine1Base(
             int          screenWidth,
             int          screenHeight,
             std::string  title = "Engine1",
             bool         isWindowed = true,
             int          numSamples = 8
         );

         /**
          * @brief Engine1Base destructor
         */
         virtual ~Engine1Base(
         );

         /**
          * @brief Starts Engine's work - calls method for resources allocation, starts game loop, and after it ends - calls method for resources deallocation
         */
         void start(
         );

         /**
          * @brief Loads new Model from file and adds it to models hashmap
          * @param path Path to model file
          * @param modelId ID assigned to new Model object in Engine
         */
         void addModel(
             std::string const &path,
             uint32_t modelId
         );

         /**
          * @brief Adds new normal Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param rotationAngle Object's rotation in degrees
          * @param rotationAxis Object's rotation axis
          * @return Reference to added Object in Scene's container
         */
         Object &addNormalObject(
             uint32_t modelID,
             glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
             float scale = 1.0f,
             float rotationAngle = 0.0f,
             glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
         );

         /**
          * @brief Adds new normal Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param quat Object's rotation quaternion
          * @return Reference to added Object in Scene's container
         */
         Object &addNormalObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             glm::quat quat
         );

         /**
          * @brief Adds new normal Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param xAngle Object's rotation around X axis in degrees - YXZ rotation order
          * @param yAngle Object's rotation around Y axis in degrees - YXZ rotation order
          * @param zAngle Object's rotation around Z axis in degrees - YXZ rotation order
          * @return Reference to added Object in Scene's container
         */
         Object &addNormalObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             float xAngle,
             float yAngle,
             float zAngle
         );

         /**
          * @brief Adds new normal Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param eulerAngles Object's rotation angles around X, Y, Z in degrees - YXZ rotation order
          * @return Reference to added Object in Scene's container 
         */
         Object &addNormalObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             glm::vec3 eulerAngles
         );

         /**
          * @brief Adds new transparent Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param rotationAngle Object's rotation in degrees
          * @param rotationAxis Object's rotation axis
          * @return Reference to added Object in Scene's container
         */
         Object &addTransparentObject(
             uint32_t modelID,
             glm::vec3 pos =
             glm::vec3{ 0.0f, 0.0f, 0.0f },
             float scale = 1.0f,
             float rotationAngle = 0.0f,
             glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
         );

         /**
          * @brief Adds new transparent Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param quat Object's rotation quaternion
          * @return Reference to added Object in Scene's container
         */
         Object &addTransparentObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             glm::quat quat
         );

         /**
          * @brief Adds new transparent Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param xAngle Object's rotation around X axis in degrees - YXZ rotation order
          * @param yAngle Object's rotation around Y axis in degrees - YXZ rotation order
          * @param zAngle Object's rotation around Z axis in degrees - YXZ rotation order
          * @return Reference to added Object in Scene's container
         */
         Object &addTransparentObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             float xAngle,
             float yAngle,
             float zAngle
         );

         /**
          * @brief Adds new transparent Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param eulerAngles Object's rotation angles around X, Y, Z in degrees - YXZ rotation order
          * @return Reference to added Object in Scene's container
         */
         Object &addTransparentObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             glm::vec3 eulerAngles
         );

         /**
          * @brief Adds new mirror Object with empty Cubemap into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param rotationAngle Object's rotation in degrees
          * @param rotationAxis Object's rotation axis
          * @return Reference to added Object and it's empty Cubemap in Scene's container
         */
         pMirrorCubemap &addMirrorObject(
             uint32_t modelID,
             glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
             float scale = 1.0f,
             float rotationAngle = 0.0f,
             glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
         );

         /**
          * @brief Adds new mirror Object with empty Cubemap into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param quat Object's rotation quaternion
          * @return Reference to added Object and it's empty Cubemap in Scene's container
         */
         pMirrorCubemap &addMirrorObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             glm::quat quat
         );

         /**
          * @brief Adds new mirror Object with empty Cubemap into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param xAngle Object's rotation around X axis in degrees - YXZ rotation order
          * @param yAngle Object's rotation around Y axis in degrees - YXZ rotation order
          * @param zAngle Object's rotation around Z axis in degrees - YXZ rotation order
          * @return Reference to added Object and it's empty Cubemap in Scene's container
         */
         pMirrorCubemap &addMirrorObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             float xAngle,
             float yAngle,
             float zAngle
         );

         /**
          * @brief Adds new mirror Object with empty Cubemap into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param eulerAngles Object's rotation angles around X, Y, Z in degrees - YXZ rotation order
          * @return Reference to added Object and it's empty Cubemap in Scene's container
         */
         pMirrorCubemap &addMirrorObject(
             uint32_t modelID,
             glm::vec3 pos,
             float scale,
             glm::vec3 eulerAngles
         );

         /**
          * @brief Adds new instanced Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param numInstances Number of Object's instances
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param rotationAngle Object's rotation in degrees
          * @param rotationAxis Object's rotation axis
          * @return Reference to added Object and it's number of instances in Scene's container
         */
         pObjInstancesNum &addInstancedObject(
             uint32_t modelID,
             uint32_t numInstances,
             glm::vec3 pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
             float scale = 1.0f,
             float rotationAngle = 0.0f,
             glm::vec3 rotationAxis = glm::vec3{ 0.0f, 1.0f, 0.0f }
         );

         /**
          * @brief Adds new instanced Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param numInstances Number of Object's instances
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param quat Object's rotation quaternion
          * @return Reference to added Object and it's number of instances in Scene's container
         */
         pObjInstancesNum &addInstancedObject(
             uint32_t modelID,
             uint32_t numInstances,
             glm::vec3 pos,
             float scale,
             glm::quat quat
         );

         /**
          * @brief Adds new instanced Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param numInstances Number of Object's instances
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param xAngle Object's rotation around X axis in degrees - YXZ rotation order
          * @param yAngle Object's rotation around Y axis in degrees - YXZ rotation order
          * @param zAngle Object's rotation around Z axis in degrees - YXZ rotation order
          * @return Reference to added Object and it's number of instances in Scene's container
         */
         pObjInstancesNum &addInstancedObject(
             uint32_t modelID,
             uint32_t numInstances,
             glm::vec3 pos,
             float scale,
             float xAngle,
             float yAngle,
             float zAngle
         );

         /**
          * @brief Adds new instanced Object into Engine's Scene
          * @param modelID ID of Model used by Object
          * @param numInstances Number of Object's instances
          * @param pos Object's position in world space
          * @param scale Object's scale
          * @param eulerAngles Object's rotation angles around X, Y, Z in degrees - YXZ rotation order
          * @return Reference to added Object and it's number of instances in Scene's container
         */
         pObjInstancesNum &addInstancedObject(
             uint32_t modelID,
             uint32_t numInstances,
             glm::vec3 pos,
             float scale,
             glm::vec3 eulerAngles
         );

         /**
          * @brief Adds new DirectionalLight and it's empty ShadowMap into Engine's Scene
          * @param direction Direction of lights rays
          * @param color Color of light represented as floats 
          * @return Reference to added DirectionalLight and it's empty ShadowMap in Scene's container
         */
         pDirLightShadow &addDirLight(
             glm::vec3 direction,
             glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)
         );

         /**
          * @brief Adds new PointLight and it's empty ShadowCubemap into Engine's Scene
          * @param position Position of light in world space
          * @param color Color of light represented as floats 
          * @param attenuationConst Attenuation constant term
          * @param attenuationLinear Attenuation linear term
          * @param attenuationQuadratic Attenuation quadratic term
          * @return Reference to added PointLight and it's empty ShadowCubemap in Scene's container
         */
         pPointLightShadows &addPointLight(
             glm::vec3 position,
             glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
             float attenuationConst = 1.0f,
             float attenuationLinear = 0.09f,
             float attenuationQuadratic = 0.032f
         );

         /**
          * @brief Adds new SpotLight and it's empty ShadowMap into Engine's Scene
          * @param position Position of light in world space
          * @param direction Direction of lights rays
          * @param color Color of light represented as floats 
          * @param cutOffAngle Inner cutoff angle in degrees
          * @param cutOffOuterAngle Outer cutoff angle in degrees
          * @param attenuationConst Attenuation constant term
          * @param attenuationLinear Attenuation linear term
          * @param attenuationQuadratic Attenuation quadratic term
          * @return Reference to added SpotLight and it's empty ShadowMap in Scene's container
         */
         pSpotLightShadows &addSpotLight(
             glm::vec3 position,
             glm::vec3 direction,
             glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
             float cutOffAngle = 8.0f,
             float cutOffOuterAngle = 12.0f,
             float attenuationConst = 1.0f,
             float attenuationLinear = 0.09f,
             float attenuationQuadratic = 0.032f
         );

         /**
          * @brief Sets color used to fill screen when clearing it from previous frame
          * @param color RGBA color represented as floats 
         */
         void setClearColor(
             glm::vec4 color
         );

         /**
          * @brief Sets color used to fill screen when clearing it from previous frame
          * @param r Red color component represented as float
          * @param g Green color component represented as float
          * @param b Blue color component represented as float
          * @param a Alpha color component represented as float
         */
         void setClearColor(
             float r = 1.0f,
             float g = 1.0f,
             float b = 1.0f,
             float a = 1.0f
         );

         /**
          * @brief Switches between game mode and debug mode
          * 
          * In debug mode renders GUI and changes some input handlers
          * 
          * @param isDebugMode if true - sets to debug mode, else - to game mode
         */
         void setGameOrDebugMode(
             bool isDebugMode
         );

         /**
          * @brief Loads Models and Scene data from json file
          * @param filePath Path to json file
         */
         void loadLevel(
             std::string const &filePath
         );

         /**
          * @brief Saves Models and Scene data to json file
          * @param filePath Path to json file
         */
         void saveLevel(
             std::string const &filePath
         );

         /**
          * @brief Rebuilds shadowmaps for all scene's DirectionalLight sources
         */
         void rebuildDirLightsShadows(
         );

         /**
          * @brief Rebuilds shadowmaps for all scene's PointLight sources
         */
         void rebuildPointLightsShadows(
         );

         /**
          * @brief Rebuilds shadowmaps for all scene's SpotLight sources
         */
         void rebuildSpotLightsShadows(
         );

         /**
          * @brief Rebuilds shadowmaps for all scene's light sources
         */
         void rebuildShadows(
         );

         /**
          * @brief Rebuilds cubemaps for all scene's mirror objects
         */
         void rebuildReflections(
         );

         void initPostprocessing(
             Shader &&postprocessingShader
         );

         void drawPostprocessingQuad(
         );

         virtual void loadPostprocessingShader(
         );

         PostprocessingData getPostprocessingData(
         ) const;

         void togglePostprocessing(
             bool bEnabled
         );

         void toggleHDR(
             bool bEnabled
         );

         void setHDRExposure(
             float exposure
         );

     protected:
         /**
          * @brief Abstract method used to allocate resources for Engine
          * 
          * Method is mandatory for implementing own game engine using Engine1Base class. \n
          * In this method you should create Scene, renderer objects, load Models, Objects, Skybox and light sources \n
          * from json file or create them yourself. Static ShadowMaps, ShadowCubemaps and Cubemaps can also be created in this method.
          * 
          * @return True indicates that all resources loaded correctly, False to indicate error - game loop will be skipped
         */
         virtual bool userCreate(
         ) = 0;

         /**
          * @brief Abstract method used to check per-frame-inputs and update Scene
          * 
          * Method is mandatory for implementing own game engine using Engine1Base class. \n
          * In this method per-frame inputs should be handled(with @ref processInputPerFrame method) such as camera movement and rotation, \n
          * Scene should be also re-rendered in this method with created renderer objects
          * 
          * @param elapsedTime Time in seconds between this frame and previous one
          * @return True to continue game loop, False to end it
         */
         virtual bool userFrameUpdate(
             float elapsedTime
         ) = 0;

         /**
          * @brief Method which can be overriden to clean up resources that need to be deallocated manually
          * @return True to indicate that all resources deallocated successfully, False to indicate error
         */
         virtual bool userDestroy(
         );

         /**
          * @brief Updates per-frame inputs such as camera movement and rotation
          * @param speedMult Coefficient that can be added to multiply delta values
         */
         virtual void processInputPerFrame(
             float speedMult = 1.0f
         );

         /**
          * @brief Method which is called when window gets resized
          * @param newWidth Window's new width
          * @param newHeight Window's new height
         */
         virtual void windowResizeCallback(
             int newWidth,
             int newHeight
         );

         /**
          * @brief Method which gets called on keyboard input and should be used to handle input
          * @param key Key code defined by GLFW
          * @param scancode Platform specific unique code of each key
          * @param action Key action defined by GLFW - press, release, hold
          * @param mods Indicates which modification keys were held down - shift, alt, control
          * @note Don't use this method to handle per-frame inputs - use @ref processInputPerFrame for that
         */
         virtual void keyCallback(
             int key,
             int scancode,
             int action,
             int mods
         );

         /**
          * @brief Method which gets called on mouse button input and should be used to handle input
          * @param button Button code defined by GLFW
          * @param action Button action defined by GLFW - press, release, hold
          * @param mods Indicates which modification keys were held down - shift, alt, control
         */
         virtual void mouseButtonCallback(
             int button,
             int action,
             int mods
         );

         /**
          * @brief Method which gets called on cursor reposition
          * @param xpos Cursors new X coordinate - from left to right
          * @param ypos Cursors new Y coordinate - from top to bottom
         */
         virtual void cursorRepositionCallback(
             double xpos,
             double ypos
         );

         /**
          * @brief Method which should be used in debug mode to select Objects with cursor
          * 
          * Method uses ColoredShapesRender to render normal, transparent and mirror Objects' shapes with unique colors. \n
          * After that, Object gets selected by using colored pixel pointed by cursor with CursorPicker class. \n
          * Recieved value is set into ObjectTransformWindow if it exists which allows moving, rotating and scaling selected Object.
          * 
          * @note Method is supposed to be called by handlers
         */
         void objectCursorPickerHandler(
         );

    };

} // OGL::E1

#endif // OGL_E1_H
