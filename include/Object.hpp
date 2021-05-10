/**
* @file Object.hpp
*/

#ifndef OGL_OBJECT_H
#define OGL_OBJECT_H

#include "Model.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace OGL {

    /**
     * @brief Class that represents game Object
    */
    class Object {
     protected:
        /// Reference to Model used by Object
        Model    &m_model;

        /// Object's position in world space
        glm::vec3 m_position;

        /// Object's scale
        float     m_scale;

        /// Object's rotation angles around X, Y, Z in degrees - YXZ rotation order
        glm::vec3 m_rotAngles;

        /// Object's model matrix
        glm::mat4 m_modelMatrix;

        /// Object's ID
        uint32_t  m_id;

     public:
        /**
         * @brief Object constructor
         * @param model Reference to Model used by Object
         * @param position Object's position in world space
         * @param scale Object's scale
         * @param rotationAngle Object's rotation in degrees
         * @param rotationAxis Object's rotation axis
         * @param objectID Object's ID
        */
        Object(
            Model &model, 
            glm::vec3 position = { 0.0f, 0.0f, 0.0f }, 
            float scale = 1.0f, 
            float rotationAngle = 0.0f, 
            glm::vec3 rotationAxis = { 0.0f, 1.0f, 0.0f },
            uint32_t objectID = 0
        );

        /**
         * @brief Object constructor
         * @param model Reference to Model used by Object
         * @param position Object's position in world space
         * @param scale Object's scale
         * @param quat Object's rotation quaternion
         * @param objectID Object's ID
        */
        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            glm::quat quat,
            uint32_t objectID = 0
        );

        /**
         * @brief Object constructor
         * @param model Reference to Model used by Object
         * @param position Object's position in world space
         * @param scale Object's scale
         * @param xAngle Object's rotation around X axis in degrees - YXZ rotation order
         * @param yAngle Object's rotation around Y axis in degrees - YXZ rotation order
         * @param zAngle Object's rotation around Z axis in degrees - YXZ rotation order
         * @param objectID Object's ID
        */
        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            float xAngle,
            float yAngle,
            float zAngle,
            uint32_t objectID = 0
        );

        /**
         * @brief Object constructor
         * @param model Reference to Model used by Object
         * @param position Object's position in world space
         * @param scale Object's scale
         * @param eulerAngles Object's rotation angles around X, Y, Z in degrees - YXZ rotation order
         * @param objectID Object's ID
        */
        Object(
            Model &model,
            glm::vec3 position,
            float scale,
            glm::vec3 eulerAngles,
            uint32_t objectID = 0
        );

        /**
         * @brief Renders Object using supplied Shader
         * @param shader Currently active Shader used to render Object
        */
        void draw( 
            Shader &shader
        ) const;

        /**
         * @brief Renders amount instances of Object using supplied Shader
         * @param shader Currently active Shader used to render Object
         * @param amount Number of instances
        */
        void drawInstanced( 
            Shader &shader, 
            uint32_t amount
        ) const;

        /**
         * @brief Renders Object using supplied Shader without loading textures and materials into Shader
         * @param shader Currently active Shader used to render Object
        */
        void drawShape(
            Shader &shader
        ) const;

        /**
         * @brief Renders amount instances of Object using supplied Shader without loading textures and materials into Shader
         * @param shader Currently active Shader used to render Object
         * @param amount Number of instances
        */
        void drawShapeInstanced(
            Shader &shader,
            uint32_t amount
        ) const;

        /**
         * @brief Enables vertex attribute for every Object's Model Mesh to pass Mat4x4 data into vertex Shader
         * @param attribLocation First location index used by Mat4x4 data - indexes from attribLocation to attribLocation + 3 will be occupied
        */
        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

        /**
         * @brief Get Object position in world space
         * @return Object position
        */
        glm::vec3 getPosition(
        ) const;

        /**
         * @brief Change Object position
         * @param position New position in world space
        */
        void setPosition(
            glm::vec3 position
        );

        /**
         * @brief Get Object scale
         * @return Object scale
        */
        float getScale(
        ) const;

        /**
         * @brief Change Object scale
         * @param scale New scale
        */
        void setScale (
            float scale
        );

        /**
         * @brief Get Object rotation angles around global X, Y, Z in degrees - YXZ rotation order
         * @return Object rotation angles around global X, Y, Z in degrees - YXZ rotation order
        */
        glm::vec3 getRotationAngles(
        ) const;

        /**
         * @brief Change Object rotation
         * @param rotationQuat New rotation quaternion
        */
        void setRotation(
            glm::quat rotationQuat
        );

        /**
         * @brief Change Object rotation
         * @param rotationAngle New Object rotation angle
         * @param rotationAxis New Object rotation axis
        */
        void setRotation(
            float rotationAngle,
            glm::vec3 rotationAxis
        );

        /**
         * @brief Change Object rotation
         * @param xAngle New Object's rotation around X axis in degrees - YXZ rotation order
         * @param yAngle New Object's rotation around Y axis in degrees - YXZ rotation order
         * @param zAngle New Object's rotation around Z axis in degrees - YXZ rotation order
        */
        void setRotation(
            float xAngle,
            float yAngle,
            float zAngle
        );

        /**
         * @brief Change Object rotation
         * @param eulerAngles New Object's rotation X, Y, Z, angles in degrees - YXZ rotation order
        */
        void setRotation(
            glm::vec3 eulerAngles
        );

        /**
         * @brief Get Object translation matrix to it's world space coordinates
         * @return Object translation matrix to it's world space coordinates
        */
        glm::mat4 getTranslationMatrix(
        ) const;

        /**
         * @brief Get Object rotation matrix
         * @return Object rotation matrix
        */
        glm::mat4 getRotationMatrix(
        ) const;

        /**
         * @brief Get Object scale matrix
         * @return Object scale matrix
        */
        glm::mat4 getScaleMatrix(
        ) const;

        /**
         * @brief Get Object model matrix = translation * rotation * scale
         * @return Object model matrix = translation * rotation * scale
        */
        glm::mat4 getModelMatrix(
        ) const;

        /**
         * @brief Get Object ID
         * @return Object ID
        */
        uint32_t getID(
        ) const;

        /**
         * @brief Get Object name = modelName + objectID
         * @return Object name = modelName + objectID
        */
        std::string getName(
        ) const;

        /**
         * @brief Get Full Model file path
         * @return Full Model file path
        */
        std::string getModelPath(
        ) const;

     protected:
        /**
         * @brief Recalculates Object model matrix
        */
        void recalculateModelMatrix(
        );

    };

}

#endif // OGL_OBJECT_H
