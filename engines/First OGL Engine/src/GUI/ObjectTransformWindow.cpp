#include "GUI/ObjectTransformWindow.hpp"

namespace OGL::E1::GUI {

    ObjectTransformWindow::ObjectTransformWindow(
    ) : m_obj{ nullptr } {

    }

    void ObjectTransformWindow::render(
    ) {
        if (!m_obj) {
            return;
        }

        glm::vec3 objPos = m_obj->getPosition();
        float objScale = m_obj->getScale();
        glm::vec3 objEulerAngles = m_obj->getRotationAngles();

        ImGui::Begin("GUI Window");
        ImGui::Text(m_obj->getName().c_str());
        ImGui::NewLine();
        ImGui::Text("Position");
        if (ImGui::InputFloat("X##Post", &(objPos.x))) {
            m_obj->setPosition(objPos);
        }
        if (ImGui::InputFloat("Y##Post", &(objPos.y))) {
            m_obj->setPosition(objPos);
        }
        if (ImGui::InputFloat("Z##Post", &(objPos.z))) {
            m_obj->setPosition(objPos);
        }
        ImGui::NewLine();
        ImGui::Text("Scale");
        if (ImGui::InputFloat("##Scale", &(objScale))) {
            m_obj->setScale(objScale);
        }
        ImGui::NewLine();
        ImGui::Text("Rotation (Order YXZ)");
        if (ImGui::InputFloat("Y##Rotation", &(objEulerAngles.y))) {
            m_obj->setRotation(objEulerAngles);
        }
        if (ImGui::InputFloat("X##Rotation", &(objEulerAngles.x))) {
            m_obj->setRotation(objEulerAngles);
        }
        if (ImGui::InputFloat("Z##Rotation", &(objEulerAngles.z))) {
            m_obj->setRotation(objEulerAngles);
        }
        ImGui::End();
    }

    void ObjectTransformWindow::setObject(
        Object *obj
    ) {
        m_obj = obj;
    }

} // OGL::E1::GUI