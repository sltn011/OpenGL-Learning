#include "Scene.hpp"

namespace OGL::E1 {

    Scene::Scene( 
        smartCamPtr &&camera
    ) : m_camera{ std::move(camera) } {
    }

    void Scene::addNormalObj( 
        smartObjPtr &&obj
    ) {
        m_normalObjects.emplace_back(std::move(obj));
    }

    void Scene::addTransparentObj( 
        smartObjPtr &&obj
    ) {
        m_transparentObjects.emplace_back(std::move(obj));
    }

    void Scene::addMirrorObj(
        smartObjPtr &&obj
    ) {
        std::pair<smartObjPtr, smartCubemap> pair;
        pair.first = std::move(obj);
        m_mirrorObjects.emplace_back(std::move(pair)); // Cubemap is empty first
    }

    void Scene::addDirLight( 
        DirectionalLight dirLight
    ) {
        m_dirLights.push_back(dirLight);
    }

    void Scene::addPointLight( 
        PointLight pointLight
    ) {
        m_pointLights.push_back(pointLight);
    }

    void Scene::addSpotLight( 
        SpotLight spotLight
    ) {
        m_spotLights.push_back(spotLight);
    }

    objectsVec &Scene::getNormalObjs(
    ) {
        return m_normalObjects;
    }

    objectsVec &Scene::getTransparentObjs(
    ) {
        return m_transparentObjects;
    }

    mirrorObjsVec &Scene::getMirrorObjs(
    ) {
        return m_mirrorObjects;
    }

    dirLights &Scene::getDirLights(
    ) {
        return m_dirLights;
    }

    pointLights &Scene::getPointLights(
    ) {
        return m_pointLights;
    }

    spotLights &Scene::getSpotLights(
    ) {
        return m_spotLights;
    }

    void Scene::replaceSkybox( 
        smartSkybox &&skybox
    ) {
        m_skybox = std::move(skybox);
    }

    smartSkybox &Scene::getSkybox(
    ) {
        return m_skybox;
    }

    smartCamPtr &Scene::getCamera(
    ) {
        return m_camera;
    }

} // OGL::E1