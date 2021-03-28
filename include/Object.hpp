#ifndef OGL_OBJECT_H
#define OGL_OBJECT_H

#include "Model.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

namespace OGL {

    class Object {
     public:
        Object(
            Model &model, 
            glm::vec3 position = { 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f, 
            glm::vec3 rotationAxis = { 0.0f, 1.0f, 0.0f }
        );

        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            glm::quat quat
        );

        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle
        );

        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            glm::vec3 eulerAngles
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
            float zAngle
        );

        void setRotation(
            glm::vec3 eulerAngles
        );

        glm::mat4 getModelMatrix(
        ) const;


     protected:
        Model &m_model;
        glm::vec3 m_position;
        float     m_scale;
        glm::quat m_quat;
        glm::mat4 m_modelMatrix;

        void recalculateModelMatrix(
        );

    };

}

#endif // OGL_OBJECT_H
