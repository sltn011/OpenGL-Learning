#include "SpotLight.hpp"

namespace OGL {

    SpotLight::SpotLight( 
        glm::vec3 position,
        glm::vec3 direction,
        glm::vec3 color, 
        float cutOffAngle,
        float cutOffOuterAngle,
        float attenuationConst, 
        float attenuationLinear,
        float attenuationQuadratic
    ) : PointLight{ 
        position, 
        color, 
        attenuationConst, 
        attenuationLinear,
        attenuationQuadratic}, 
        m_direction{direction}, 
        m_cutOffAngle{ cutOffAngle }, 
        m_cutOffOuterAngle{ cutOffOuterAngle } {
    }

    void SpotLight::loadInShader( 
        OGL::Shader &shader
    ) {
        std::string objName = "spotLight";
        shader.setUniformVec3(objName + ".color", m_color);
        shader.setUniformVec3(objName + ".position", m_position);
        shader.setUniformVec3(objName + ".direction", glm::normalize(m_direction));
        shader.setUniformFloat(objName + ".attenuationConst", m_attenuationConst);
        shader.setUniformFloat(objName + ".attenuationLinear", m_attenuationLinear);
        shader.setUniformFloat(objName + ".attenuationQuadratic", m_attenuationQuadratic);
        shader.setUniformFloat(objName + ".cutOffCos", std::cos(glm::radians(m_cutOffAngle)));
        shader.setUniformFloat(objName + ".cutOffOuterCos", std::cos(glm::radians(m_cutOffOuterAngle)));
    }

    void SpotLight::loadInShader( 
        OGL::Shader &shader,
        int indexInArray
    ) {
        std::string objName = "spotLight[" + std::to_string(indexInArray) + "]";
        shader.setUniformVec3(objName + ".color", m_color);
        shader.setUniformVec3(objName + ".position", m_position);
        shader.setUniformVec3(objName + ".direction", glm::normalize(m_direction));
        shader.setUniformFloat(objName + ".attenuationConst", m_attenuationConst);
        shader.setUniformFloat(objName + ".attenuationLinear", m_attenuationLinear);
        shader.setUniformFloat(objName + ".attenuationQuadratic", m_attenuationQuadratic);
        shader.setUniformFloat(objName + ".cutOffCos", glm::cos(glm::radians(m_cutOffAngle)));
        shader.setUniformFloat(objName + ".cutOffOuterCos", std::cos(glm::radians(m_cutOffOuterAngle)));
    }


} // OGL