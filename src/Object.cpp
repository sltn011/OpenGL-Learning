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
        m_id{objectID} {
        setRotation(quat);
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

    glm::vec3 Object::getRotationAngles(
    ) const {
        return m_rotAngles;
    }

    void Object::setRotation(
        glm::quat rotationQuat
    ) {
        glm::vec3 radians;
        glm::extractEulerAngleZXY(glm::toMat4(glm::normalize(rotationQuat)), radians.z, radians.x, radians.y);
        setRotation(glm::degrees(radians));
    }

    void Object::setRotation(
        float rotationAngle, 
        glm::vec3 rotationAxis
    ) {
        glm::quat rot = glm::normalize(glm::angleAxis(glm::radians(rotationAngle), glm::normalize(rotationAxis)));
        setRotation(rot);
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
        m_rotAngles = eulerAngles;
        recalculateModelMatrix();
    }

    glm::mat4 Object::getTranslationMatrix(
    ) const {
        return glm::translate(glm::identity<glm::mat4>(), m_position);
    }

    glm::mat4 Object::getRotationMatrix(
    ) const {
        glm::vec3 radiansVec = glm::radians(m_rotAngles);
        glm::quat xRot = glm::normalize(glm::angleAxis(radiansVec.x, glm::vec3{ 1.0f, 0.0f, 0.0f }));
        glm::quat yRot = glm::normalize(glm::angleAxis(radiansVec.y, glm::vec3{ 0.0f, 1.0f, 0.0f }));
        glm::quat zRot = glm::normalize(glm::angleAxis(radiansVec.z, glm::vec3{ 0.0f, 0.0f, 1.0f }));
        glm::quat rot = glm::normalize(zRot * xRot * yRot);
        return glm::toMat4(rot);
    }

    glm::mat4 Object::getScaleMatrix(
    ) const {
        return glm::scale(glm::identity<glm::mat4>(), glm::vec3{ m_scale, m_scale, m_scale });
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
        glm::mat4 translation = getTranslationMatrix();
        glm::mat4 rotation = getRotationMatrix();
        glm::mat4 scale = getScaleMatrix();

        m_modelMatrix = translation * rotation * scale;
    }

}// OGL