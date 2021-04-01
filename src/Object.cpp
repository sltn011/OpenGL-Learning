#include "Object.hpp"

namespace OGL {

    Object::Object( 
        Model &model, 
        glm::vec3 position,
        float scale,
        float rotationAngle,
        glm::vec3 rotationAxis,
        size_t objectID
    ) : m_model{model},
        m_position{position},
        m_scale{scale},
        m_id{objectID} {
        setRotation(rotationAngle, rotationAxis);
    }

    Object::Object(
        Model &model, 
        glm::vec3 position,
        float scale, 
        glm::quat quat,
        size_t objectID
    ) : m_model{model},
        m_position{position},
        m_scale{scale},
        m_quat{quat},
        m_id{objectID} {
        recalculateModelMatrix();
    }

    Object::Object(
        Model &model, 
        glm::vec3 position, 
        float scale, 
        float xAngle,
        float yAngle,
        float zAngle,
        size_t objectID
    ) : m_model{model},
        m_position{position},
        m_scale{scale},
        m_id{objectID} {
        setRotation(xAngle, yAngle, zAngle);
    }

    Object::Object(
        Model &model,
        glm::vec3 position,
        float scale,
        glm::vec3 eulerAngles,
        size_t objectID
    ) : m_model{model},
        m_position{position},
        m_scale{scale},
        m_id{objectID} {
        setRotation(eulerAngles);
    }

    void Object::draw( 
        Shader &shader
    ) const {
        shader.setUniformMatrix4("model", m_modelMatrix);
        m_model.draw(shader);
    }

    void Object::drawInstanced( 
        Shader &shader, 
        size_t amount
    ) const {
        shader.setUniformMatrix4("model", m_modelMatrix);
        m_model.drawInstanced(shader, amount);
    }

    void Object::drawShape(
        Shader &shader
    ) const {
        shader.setUniformMatrix4("model", m_modelMatrix);
        m_model.drawShape(shader);
    }

    void Object::drawShapeInstanced(
        Shader &shader, 
        size_t amount
    ) const {
        shader.setUniformMatrix4("model", m_modelMatrix);
        m_model.drawShapeInstanced(shader, amount);
    }

    void Object::setVertexAttribInstancedModelMat4( 
        int attribLocation
    ) {
        m_model.setVertexAttribInstancedModelMat4(attribLocation);
    }

    glm::vec3 Object::getPosition(
    ) const {
        return m_position;
    }

    void Object::setPosition(
        glm::vec3 position
    ) {
        m_position = position;
        recalculateModelMatrix();
    }

    float Object::getScale(
    ) const {
        return m_scale;
    }

    void Object::setScale(
        float scale
    ) {
        m_scale = scale;
        recalculateModelMatrix();
    }

    glm::quat Object::getRotation(
    ) const {
        return m_quat;
    }

    void Object::setRotation(
        glm::quat rotationQuat
    ) {
        m_quat = glm::normalize(rotationQuat);
        recalculateModelMatrix();
    }

    void Object::setRotation(
        float rotationAngle, 
        glm::vec3 rotationAxis
    ) {
        m_quat = glm::normalize(glm::angleAxis(glm::radians(rotationAngle), glm::normalize(rotationAxis)));
        recalculateModelMatrix();
    }

    void Object::setRotation(
        float xAngle, 
        float yAngle, 
        float zAngle
    ) {
        glm::vec3 anglesVec{ xAngle, yAngle, zAngle };
        setRotation(anglesVec);
    }

    void Object::setRotation(
        glm::vec3 eulerAngles
    ) {
        glm::vec3 radiansVec = glm::radians(eulerAngles);
        m_quat = glm::normalize(glm::quat(radiansVec));
        recalculateModelMatrix();
    }

    glm::mat4 Object::getModelMatrix(
    ) const {
        return m_modelMatrix;
    }

    size_t Object::getID(
    ) const {
        return m_id;
    }

    std::string Object::getName(
    ) const {
        return m_model.getName() + std::to_string(m_id);
    }

    void Object::recalculateModelMatrix(
    ) {
        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 translation = glm::translate(model, m_position);
        glm::mat4 rotation = glm::toMat4(glm::normalize(m_quat));
        glm::mat4 scale = glm::scale(model, glm::vec3{ m_scale, m_scale, m_scale });

        m_modelMatrix = translation * rotation * scale;
    }

}// OGL