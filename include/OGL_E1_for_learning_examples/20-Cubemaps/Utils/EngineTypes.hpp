#ifndef OGL_E1UTILS_H
#define OGL_E1UTILS_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

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

    class BasicCamera;
    class CameraFirstPerson;
    class CameraFree;

    class Mesh;
    class Model;
    class Object;

    class BasicLight;
    class DirectionalLight;
    class PointLight;
    class SpotLight;

    namespace E1 {

        using smartDescriptor = std::unique_ptr<Descriptor>;
        using smartVAO = std::unique_ptr<VertexArrayObject>;
        using smartVBO = std::unique_ptr<VertexBufferObject>;
        using smartEBO = std::unique_ptr<ElementBufferObject>;
        using smartCBO = std::unique_ptr<ColorBufferObject>;
        using smartRBO = std::unique_ptr<RenderBufferObject>;
        using smartFBO = std::unique_ptr<FrameBufferObject>;
        using smartCubemap = std::unique_ptr<Cubemap>;

        using smartCamPtr = std::unique_ptr<BasicCamera>;

        using smartModelPtr = std::unique_ptr<Model>;
        using smartObjPtr = std::unique_ptr<Object>;

        using smartDirLPtr = std::unique_ptr<DirectionalLight>;
        using smartPointLPtr = std::unique_ptr<PointLight>;
        using smartSpotLPtr = std::unique_ptr<SpotLight>;
        

        using modelsTable = std::unordered_map<size_t, smartModelPtr>;
        using objectsGroup = std::vector<smartObjPtr>;
        using gameObjects = std::unordered_map<size_t, objectsGroup>;

        using dirLights = std::vector<smartDirLPtr>;
        using pointLights = std::vector<smartPointLPtr>;
        using spotLights = std::vector<smartSpotLPtr>;

        
        using shadersPack = std::vector<Shader>;

    } // E1
    
} // OGL

#endif // OGL_E1UTILS_H