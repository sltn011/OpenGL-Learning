#include "BasicCamera.hpp"

namespace OGL {

    BasicCamera::BasicCamera( 
        glm::vec3 positionVec,
        glm::vec3 forwardVec,
        glm::vec3 worldUpVec,
        float moveSpeed,
        float yawAngle,
        float pitchAngle,
        float fov,
        float aspect,
        float near,
        float far
    ) : m_position{ positionVec }, 
        m_forward{ forwardVec }, 
        m_worldUp{ worldUpVec }, 
        m_speed{ moveSpeed }, 
        m_yaw{ yawAngle }, 
        m_pitch{ pitchAngle }, 
        m_fov{ fov }, 
        m_aspect{ aspect }, 
        m_near{ near }, 
        m_far{ far } {
    }

    BasicCamera::~BasicCamera(
    ) {
    }

    glm::mat4 BasicCamera::getViewMatrix(
    ) const {
        return glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    glm::mat4 BasicCamera::getProjectionMatrix(
    ) const {
        return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    glm::vec3 BasicCamera::getPos(
    ) const {
        return m_position;
    }

    glm::vec3 BasicCamera::getForward(
    ) const {
        return m_forward;
    }

    glm::vec3 BasicCamera::getUp(
    ) const {
        return m_up;
    }

    void BasicCamera::updateVectors(
    ) {
        m_forward.x = std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));
        m_forward.y = std::sin(glm::radians(m_pitch));
        m_forward.z = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));
        m_forward = glm::normalize(m_forward);

        m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }

}