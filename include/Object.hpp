#ifndef OGL_OBJECT_H
#define OGL_OBJECT_H

#include "Model.hpp"
#include "RotationOrder.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace OGL {

    class Object {
     public:
        Object(
            Model &model, 
            glm::vec3 position = { 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f, 
            glm::vec3 rotationAxis = { 0.0f, 1.0f, 0.0f },
            size_t objectID = 0
        );

        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            glm::quat quat,
            size_t objectID = 0
        );

        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle,
            RotationOrder rotOrd,
            size_t objectID = 0
        );

        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            glm::vec3 eulerAngles,
            RotationOrder rotOrd,
            size_t objectID = 0
        );

        void draw( 
            Shader &shader
        ) const;

        void drawInstanced( 
            Shader &shader, 
            size_t amount
        ) const;

        void drawShape(
            Shader &shader
        ) const;

        void drawShapeInstanced(
            Shader &shader,
            size_t amount
        ) const;

        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

        glm::vec3 getPosition(
        ) const;

        void setPosition(
            glm::vec3 position
        );

        float getScale(
        ) const;

        void setScale (
            float scale
        );

        glm::quat getRotation(
        ) const;

        void setRotation(
            glm::quat rotationQuat
        );

        void setRotation(
            float rotationAngle,
            glm::vec3 rotationAxis
        );

        void setRotation(
            float xAngle,
            float yAngle,
            float zAngle,
            RotationOrder rotOrder
        );

        void setRotation(
            glm::vec3 eulerAngles,
            RotationOrder rotOrder
        );

        glm::mat4 getTranslationMatrix(
        ) const;

        glm::mat4 getRotationMatrix(
        ) const;

        glm::mat4 getScaleMatrix(
        ) const;

        glm::mat4 getModelMatrix(
        ) const;

        glm::vec3 getRotationAngles(
            RotationOrder rotOrder
        ) const;

        size_t getID(
        ) const;

        std::string getName(
        ) const;


     protected:
        Model &m_model;
        glm::vec3 m_position;
        float     m_scale;
        glm::quat m_quat;
        glm::mat4 m_modelMatrix;
        size_t    m_id;

        void recalculateModelMatrix(
        );

    };

}

#endif // OGL_OBJECT_H
