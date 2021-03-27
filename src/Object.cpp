#include "Object.hpp"

namespace OGL {

    Object::Object( 
        Model &model, 
        glm::vec3 position,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis
    ) : m_model{model},
        m_postiton{position}, 
        m_scale{scale},
        m_quat{glm::angleAxis(glm::radians(rotationAngle), glm::normalize(rotationAxis))} {
    }

    Object::Object(
        Model &model, 
        glm::vec3 position,
        float scale, 
        glm::quat quat
    ) : m_model{model},
        m_postiton{position},
        m_scale{scale},
        m_quat{quat} {
    }

    void Object::draw( 
        Shader &shader
    ) const {
        glm::vec3 scaleVec = { m_scale, m_scale, m_scale };

        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 translation = glm::translate(model, m_postiton);
        glm::mat4 rotation    = glm::toMat4(glm::normalize(m_quat));
        glm::mat4 scale       = glm::scale(model, scaleVec);
        shader.setUniformMatrix4("model", translation * rotation * scale);
        m_model.draw(shader);
    }

    void Object::drawInstanced( 
        Shader &shader, 
        size_t amount
    ) const {
        glm::vec3 scaleVec = { m_scale, m_scale, m_scale };

        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 translation = glm::translate(model, m_postiton);
        glm::mat4 rotation    = glm::toMat4(glm::normalize(m_quat));
        glm::mat4 scale       = glm::scale(model, scaleVec);
        shader.setUniformMatrix4("model", translation * rotation * scale);
        m_model.drawInstanced(shader, amount);
    }

    void Object::drawShape(
        Shader &shader
    ) const {
        glm::vec3 scaleVec = { m_scale, m_scale, m_scale };

        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 translation = glm::translate(model, m_postiton);
        glm::mat4 rotation    = glm::toMat4(glm::normalize(m_quat));
        glm::mat4 scale       = glm::scale(model, scaleVec);
        shader.setUniformMatrix4("model", translation * rotation * scale);
        m_model.drawShape(shader);
    }

    void Object::drawShapeInstanced(
        Shader &shader, 
        size_t amount
    ) const {
        glm::vec3 scaleVec = { m_scale, m_scale, m_scale };

        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 translation = glm::translate(model, m_postiton);
        glm::mat4 rotation    = glm::toMat4(glm::normalize(m_quat));
        glm::mat4 scale       = glm::scale(model, scaleVec);
        shader.setUniformMatrix4("model", translation * rotation * scale);
        m_model.drawShapeInstanced(shader, amount);
    }

    void Object::setVertexAttribInstancedModelMat4( 
        int attribLocation
    ) {
        m_model.setVertexAttribInstancedModelMat4(attribLocation);
    }

}// OGL