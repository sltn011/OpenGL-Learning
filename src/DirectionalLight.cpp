#include "DirectionalLight.hpp"

namespace OGL {

    DirectionalLight::DirectionalLight
    ( glm::vec3 direction
    , glm::vec3 color
    )
    : BasicLight{ color }
    , m_direction{ direction } {
    }

    void DirectionalLight::loadInShader
    ( OGL::Shader &shader
    ) {
        std::string objName = "directionalLight";
        shader.setUniformVec3(objName + ".color", m_color);
        shader.setUniformVec3(objName + ".direction", glm::normalize(m_direction));
    }

    void DirectionalLight::loadInShader
    ( OGL::Shader &shader
    , int indexInArray
    ) {
        std::string objName = "directionalLight[" + std::to_string(indexInArray) + "]";
        shader.setUniformVec3(objName + ".color", m_color);
        shader.setUniformVec3(objName + ".direction", glm::normalize(m_direction));
    }

} // OGL