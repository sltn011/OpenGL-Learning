#ifndef OGL_E1UTILS_H
#define OGL_E1UTILS_H

#include "Events.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

namespace OGL::E1 {

    using smartCamPtr   = std::unique_ptr<BasicCamera>;
    using smartModelPtr = std::unique_ptr<Model>;
    using smartObjPtr   = std::unique_ptr<Object>;

    using modelsTable   = std::unordered_map<uint32_t, smartModelPtr>;
    using gameObjects   = std::vector<smartObjPtr>;

    using eventsQueue   = std::queue<Events>;

} // OGL::E1

#endif // OGL_E1UTILS_H