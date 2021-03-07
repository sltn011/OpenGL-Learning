#ifndef OGL_E1UTILS_H
#define OGL_E1UTILS_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

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
        using smartScenePtr = std::unique_ptr<Scene>;

        class NormalRenderer;
        using smartNormalRendererPtr = std::unique_ptr<NormalRenderer>;
        class TransparentRenderer;
        using smartTransparentRendererPtr = std::unique_ptr<TransparentRenderer>;
        class CubemapRenderer;
        using smartCubemapRendererPtr = std::unique_ptr<CubemapRenderer>;
        class MirrorRenderer;
        using smartMirrorRendererPtr = std::unique_ptr<MirrorRenderer>;
        class SkyboxRenderer;
        using smartSkyboxRendererPtr = std::unique_ptr<SkyboxRenderer>;
        class InstancesRenderer;
        using smartInstancesRendererPtr = std::unique_ptr<InstancesRenderer>;
        class ShadowMapRenderer;
        using smartShadowMapRendererPtr = std::unique_ptr<ShadowMapRenderer>;
        class ShadowCubemapRenderer;
        using smartShadowCubemapRendererPtr = std::unique_ptr<ShadowCubemapRenderer>;
        class LightSourcesDebugRenderer;
        using smartLightSourcesDebugRendererPtr = std::unique_ptr<LightSourcesDebugRenderer>;

        using smartDescriptor = std::unique_ptr<Descriptor>;
        using smartVAO = std::unique_ptr<VertexArrayObject>;
        using smartVBO = std::unique_ptr<VertexBufferObject>;
        using smartEBO = std::unique_ptr<ElementBufferObject>;
        using smartCBO = std::unique_ptr<ColorBufferObject>;
        using smartRBO = std::unique_ptr<RenderBufferObject>;
        using smartFBO = std::unique_ptr<FrameBufferObject>;
        using smartCubemap = std::unique_ptr<Cubemap>;
        using smartSkybox = std::unique_ptr<Skybox>;
        using smartShadowMap = std::unique_ptr<ShadowMap>;
        using smartShadowCubemap = std::unique_ptr<ShadowCubemap>;

        using smartCamPtr = std::unique_ptr<BasicCamera>;
        using smartFPCamPtr = std::unique_ptr<CameraFirstPerson>;
        using smartFreeCamPtr = std::unique_ptr<CameraFree>;
        using smartCubemapCamPtr = std::unique_ptr<CameraCubemap>;

        using smartModelPtr = std::unique_ptr<Model>;
        using smartObjPtr = std::unique_ptr<Object>;

        using smartDirLPtr = std::unique_ptr<DirectionalLight>;
        using smartPointLPtr = std::unique_ptr<PointLight>;
        using smartSpotLPtr = std::unique_ptr<SpotLight>;

        using modelsTable = std::unordered_map<size_t, Model>;
        using objectsVec = std::vector<Object>;
        using mirrorObjsVec = std::vector<std::pair<Object, smartCubemap>>;
        using instancedObjsVec = std::vector<std::pair<Object, size_t>>;
        using gameObjects = std::unordered_map<size_t, objectsVec>;

        using dirLights = std::vector<std::pair<DirectionalLight, smartShadowMap>>;
        using pointLights = std::vector<std::pair<PointLight, smartShadowCubemap>>;
        using spotLights = std::vector<std::pair<SpotLight, smartShadowMap>>;

        using shadersPack = std::vector<Shader>;

        enum Events;
        using eventsQueue = std::queue<Events>;

    } // E1
    
} // OGL

#endif // OGL_E1UTILS_H