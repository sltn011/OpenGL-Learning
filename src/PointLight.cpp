#include "PointLight.hpp"

namespace OGL {

    PointLight::PointLight
    ( glm::vec3 position
    , glm::vec3 color
    , float attenuationConst
    , float attenuationLinear
    , float attenuationQuadratic
    )
    : BasicLight{ color }
    , m_position{ position }
    , m_attenuationConst{ attenuationConst }
    , m_attenuationLinear{ attenuationLinear }
    , m_attenuationQuadratic{ attenuationQuadratic }
    {
    }

    void PointLight::loadInShader
    ( OGL::Shader &shader
    ) {
        std::string objName = "pointLight";
        shader.setUniformVec3(objName + ".color", m_color);
        shader.setUniformVec3(objName + ".position", m_position);
        shader.setUniformFloat(objName + ".attenuationConst", m_attenuationConst);
        shader.setUniformFloat(objName + ".attenuationLinear", m_attenuationLinear);
        shader.setUniformFloat(objName + ".attenuationQuadratic", m_attenuationQuadratic);
    }

    void PointLight::loadInShader
    ( OGL::Shader &shader
    , int indexInArray
    ) {
        std::string objName = "pointLight[" + std::to_string(indexInArray) + "]";
        shader.setUniformVec3(objName + ".color", m_color);
        shader.setUniformVec3(objName + ".position", m_position);
        shader.setUniformFloat(objName + ".attenuationConst", m_attenuationConst);
        shader.setUniformFloat(objName + ".attenuationLinear", m_attenuationLinear);
        shader.setUniformFloat(objName + ".attenuationQuadratic", m_attenuationQuadratic);
    }


} // OGL