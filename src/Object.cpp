#include "Object.hpp"

namespace OGL {

    Object::Object
    ( Model &model
    , glm::vec3 position
    , float scale
    , float rotationAngleRadians
    , glm::vec3 rotationAxis
    )
    : m_model{model}
    , m_postiton{position}
    , m_scale{scale}
    , m_rotationAngleRadians{rotationAngleRadians}
    , m_rotationAxis{rotationAxis}
    {
    }

    void Object::draw
    ( Shader &shader
    ) const {
        glm::vec3 scaleVec = { m_scale, m_scale, m_scale };

        glm::mat4 model = glm::identity<glm::mat4>();
        model = glm::translate(model, m_postiton);
        model = glm::rotate(model, m_rotationAngleRadians, glm::normalize(m_rotationAxis));
        model = glm::scale(model, scaleVec);
        shader.setUniformMatrix4("model", model);
        m_model.draw(shader);
    }

}// OGL