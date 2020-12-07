#ifndef OGL_E1UTILS_H
#define OGL_E1UTILS_H

#include "Events.hpp"
#include "BasicCamera.hpp"
#include "Shader.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"
#include "PointLight.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

namespace OGL::E1 {

    using smartCamPtr    = std::unique_ptr<BasicCamera>;
    using smartModelPtr  = std::unique_ptr<Model>;
    using smartObjPtr    = std::unique_ptr<Object>;
    using smartDirLPtr   = std::unique_ptr<DirectionalLight>;
    using smartPointLPtr = std::unique_ptr<PointLight>;
    using smartSpotLPtr  = std::unique_ptr<SpotLight>;

    using modelsTable    = std::unordered_map<uint32_t, smartModelPtr>;
    using gameObjects    = std::vector<smartObjPtr>;
    using dirLights      = std::vector<smartDirLPtr>;
    using pointLights    = std::vector<smartPointLPtr>;
    using spotLights     = std::vector<smartSpotLPtr>;
    using shadersPack    = std::vector<Shader>;
                         
    using eventsQueue    = std::queue<Events>;

} // OGL::E1

#endif // OGL_E1UTILS_H