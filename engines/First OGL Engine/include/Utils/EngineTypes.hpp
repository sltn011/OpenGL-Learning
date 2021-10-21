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

        using smartCamPtr        = std::unique_ptr<BasicCamera>;
                                 
        using pMirrorCubemap     = std::pair<Object, maybeCubemap>;
        using pObjInstancesNum   = std::pair<Object, uint32_t>;

        using smartModelPtr      = std::unique_ptr<Model>;
                                 
        using modelsTable        = std::unordered_map<uint32_t, smartModelPtr>;
        using objectsIDs         = std::unordered_map<uint32_t, uint32_t>;
        using objectsVec         = std::vector<Object>;
        using mirrorObjsVec      = std::vector<pMirrorCubemap>;
        using instancedObjsVec   = std::vector<pObjInstancesNum>;

        using pDirLightShadow    = std::pair<DirectionalLight, maybeShadowMap>;
        using pPointLightShadows = std::pair<PointLight, maybeShadowCubemap>;
        using pSpotLightShadows  = std::pair<SpotLight, maybeShadowMap>;

        using dirLights          = std::vector<pDirLightShadow>;
        using pointLights        = std::vector<pPointLightShadows>;
        using spotLights         = std::vector<pSpotLightShadows>;
                                 
        using shadersPack        = std::vector<Shader>;

        namespace GUI {
            
            class GUIRenderer;
            using maybeGUIRenderer = std::optional<GUIRenderer>;

        } // GUI

    } // E1
    
} // OGL

#endif // OGL_E1UTILS_H