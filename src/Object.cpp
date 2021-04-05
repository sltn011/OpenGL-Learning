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
        RotationOrder rotOrd,
        size_t objectID
    ) : m_model{model},
        m_position{position},
        m_scale{scale},
        m_id{objectID} {
        setRotation(xAngle, yAngle, zAngle, rotOrd);
    }

    Object::Object(
        Model &model,
        glm::vec3 position,
        float scale,
        glm::vec3 eulerAngles,
        RotationOrder rotOrd,
        size_t objectID
    ) : m_model{model},
        m_position{position},
        m_scale{scale},
        m_id{objectID} {
        setRotation(eulerAngles, rotOrd);
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
        float zAngle,
        RotationOrder rotOrder
    ) {
        glm::vec3 anglesVec{ xAngle, yAngle, zAngle };
        setRotation(anglesVec, rotOrder);
    }

    void Object::setRotation(
        glm::vec3 eulerAngles,
        RotationOrder rotOrder
    ) {
        glm::vec3 radiansVec = glm::radians(eulerAngles);
        glm::quat xRot = glm::normalize(glm::angleAxis(radiansVec.x, glm::vec3{ 1.0f, 0.0f, 0.0f }));
        glm::quat yRot = glm::normalize(glm::angleAxis(radiansVec.y, glm::vec3{ 0.0f, 1.0f, 0.0f }));
        glm::quat zRot = glm::normalize(glm::angleAxis(radiansVec.z, glm::vec3{ 0.0f, 0.0f, 1.0f }));
        switch (rotOrder) {
        case OGL::RotationOrder::XYZ:
            m_quat = glm::normalize(zRot * yRot * xRot);
            break;
        case OGL::RotationOrder::XZY:
            m_quat = glm::normalize(yRot * zRot * xRot);
            break;
        case OGL::RotationOrder::YXZ:
            m_quat = glm::normalize(zRot * xRot * yRot);
            break;
        case OGL::RotationOrder::YZX:
            m_quat = glm::normalize(xRot * zRot * yRot);
            break;
        case OGL::RotationOrder::ZXY:
            m_quat = glm::normalize(yRot * xRot * zRot);
            break;
        case OGL::RotationOrder::ZYX:
            m_quat = glm::normalize(xRot * yRot * zRot);
            break;
        default:
            m_quat = glm::normalize(zRot * xRot * yRot);
            break;
        }
        recalculateModelMatrix();
    }

    glm::mat4 Object::getTranslationMatrix(
    ) const {
        return glm::translate(glm::identity<glm::mat4>(), m_position);
    }

    glm::mat4 Object::getRotationMatrix(
    ) const {
        return glm::toMat4(m_quat);
    }

    glm::mat4 Object::getScaleMatrix(
    ) const {
        return glm::scale(glm::identity<glm::mat4>(), glm::vec3{ m_scale, m_scale, m_scale });
    }

    glm::mat4 Object::getModelMatrix(
    ) const {
        return m_modelMatrix;
    }

    glm::vec3 Object::getRotationAngles(
        RotationOrder rotOrder
    ) const {
        glm::mat4 rotMatr = getRotationMatrix();
        glm::vec3 radians;
        switch (rotOrder) {
        case OGL::RotationOrder::XYZ:
            glm::extractEulerAngleZYX(rotMatr, radians.z, radians.y, radians.x);
            break;
        case OGL::RotationOrder::XZY:
            glm::extractEulerAngleYZX(rotMatr, radians.y, radians.z, radians.x);
            break;
        case OGL::RotationOrder::YXZ:
            glm::extractEulerAngleZXY(rotMatr, radians.z, radians.x, radians.y);
            break;
        case OGL::RotationOrder::YZX:
            glm::extractEulerAngleXZY(rotMatr, radians.x, radians.z, radians.y);
            break;
        case OGL::RotationOrder::ZXY:
            glm::extractEulerAngleYXZ(rotMatr, radians.y, radians.x, radians.z);
            break;
        case OGL::RotationOrder::ZYX:
            glm::extractEulerAngleXYZ(rotMatr, radians.x, radians.y, radians.z);
            break;
        default:
            glm::extractEulerAngleZXY(rotMatr, radians.z, radians.x, radians.y);
            break;
        }
        return glm::degrees(radians);
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