#ifndef OGL_OBJECT_H
#define OGL_OBJECT_H

#include "Model.hpp"

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

        void draw( 
            Shader &shader
        ) const;

        void drawInstanced( 
            Shader &shader, 
            size_t amount
        ) const;

        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

     protected:
        Model &m_model;

     public:
        glm::vec3 m_postiton;
        float     m_scale;
        glm::vec3 m_rotationAxis;
        float     m_rotationAngle;

    };

}

#endif // OGL_OBJECT_H
