#ifndef OGL_E1UTILS_H
#define OGL_E1UTILS_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <optional>

#include "glad/glad.h"

namespace OGL {

    class GLFWInitRaii;

    class Descriptor;
    class VertexArrayObject;
    class VertexBufferObject;
    class ElementBufferObject;
    class ColorBufferObject;
    class RenderBufferObject;
    class FrameBufferObject;
    class Cubemap;
    class Skybox;
    class ShadowMap;
    class ShadowCubemap;

    class BasicCamera;
    class CameraFirstPerson;
    class CameraFree;
    class CameraCubemap;

    class Mesh;
    class Model;
    class Object;

    class BasicLight;
    class DirectionalLight;
    class PointLight;
    class SpotLight;

    class Shader;

    namespace E1 {

        class Scene;
        using smartScenePtr                  = std::unique_ptr<Scene>;

        class NormalRenderer;
        using maybeNormalRenderer            = std::optional<NormalRenderer>;
        class TransparentRenderer;              
        using maybeTransparentRenderer       = std::optional<TransparentRenderer>;
        class CubemapRenderer;                  
        using maybeCubemapRenderer           = std::optional<CubemapRenderer>;
        class MirrorRenderer;                   
        using maybeMirrorRenderer            = std::optional<MirrorRenderer>;
        class SkyboxRenderer;                   
        using maybeSkyboxRenderer            = std::optional<SkyboxRenderer>;
        class InstancesRenderer;                
        using maybeInstancesRenderer         = std::optional<InstancesRenderer>;
        class ShadowMapRenderer;                
        using maybeShadowMapRenderer         = std::optional<ShadowMapRenderer>;
        class ShadowCubemapRenderer;
        using maybeShadowCubemapRenderer     = std::optional<ShadowCubemapRenderer>;
        class LightSourcesDebugRenderer;
        using maybeLightSourcesDebugRenderer = std::optional<LightSourcesDebugRenderer>;
        class ColoredShapesRenderer;
        using maybeColoredShapesRenderer     = std::optional<ColoredShapesRenderer>;

        using maybeDescriptor    = std::optional<Descriptor>;
        using maybeVAO           = std::optional<VertexArrayObject>;
        using maybeVBO           = std::optional<VertexBufferObject>;
        using maybeEBO           = std::optional<ElementBufferObject>;
        using maybeCBO           = std::optional<ColorBufferObject>;
        using maybeRBO           = std::optional<RenderBufferObject>;
        using maybeFBO           = std::optional<FrameBufferObject>;
        using maybeCubemap       = std::optional<Cubemap>;
        using maybeSkybox        = std::optional<Skybox>;
        using maybeShadowMap     = std::optional<ShadowMap>;
        using maybeShadowCubemap = std::optional<ShadowCubemap>;

        using smartCamPtr      = std::unique_ptr<BasicCamera>;

        using modelsTable      = std::unordered_map<uint32_t, Model>;
        using objectsIDs       = std::unordered_map<uint32_t, uint32_t>;
        using objectsVec       = std::vector<Object>;
        using mirrorObjsVec    = std::vector<std::pair<Object, maybeCubemap>>;
        using instancedObjsVec = std::vector<std::pair<Object, uint32_t>>;
        using gameObjects      = std::unordered_map<uint32_t, objectsVec>;

        using dirLights   = std::vector<std::pair<DirectionalLight, maybeShadowMap>>;
        using pointLights = std::vector<std::pair<PointLight, maybeShadowCubemap>>;
        using spotLights  = std::vector<std::pair<SpotLight, maybeShadowMap>>;

        using shadersPack = std::vector<Shader>;

        enum Events;
        using eventsQueue = std::queue<Events>;

        namespace GUI {
            
            class GUIRenderer;
            using maybeGUIRenderer = std::optional<GUIRenderer>;

        } // GUI

    } // E1
    
} // OGL

#endif // OGL_E1UTILS_H