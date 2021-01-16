#include "Scene.hpp"

namespace OGL::E1 {

    Scene::Scene
    ( smartCamPtr &&camera
    ) : m_camera{ std::move(camera) } {

    }

    void Scene::addNormalObj
    ( smartObjPtr &&obj
    ) {
        m_normalObjects.emplace_back(std::move(obj));
    }

    void Scene::addTransparentObj
    ( smartObjPtr &&obj
    ) {
        m_transparentObjects.emplace_back(std::move(obj));
    }

    void Scene::addMirrorObj
    ( smartObjPtr &&obj
    ) {
        std::pair<smartObjPtr, smartCubemap> pair;
        pair.first = std::move(obj);
        m_mirrorObjects.emplace_back(std::move(pair)); // Cubemap is empty first - will be created with first render
    }

    objectsVec &Scene::getNormalObjs
    (
    ) {
        return m_normalObjects;
    }

    objectsVec &Scene::getTransparentObjs
    (
    ) {
        return m_transparentObjects;
    }

    mirrorObjsVec &Scene::getMirrorObjs
    (
    ) {
        return m_mirrorObjects;
    }

    void Scene::replaceSkybox
    ( smartSkybox &&skybox
    ) {
        m_skybox = std::move(skybox);
    }

    smartSkybox &Scene::getSkybox
    (
    ) {
        return m_skybox;
    }

} // OGL::E1