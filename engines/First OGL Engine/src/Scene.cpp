#include "Scene.hpp"

namespace OGL::E1 {

    Scene::Scene( 
        smartCamPtr &&camera
    ) : m_camera{ std::move(camera) } {
    }

    void Scene::addNormalObj( 
        Object obj
    ) {
        m_normalObjects.emplace_back(std::move(obj));
    }

    void Scene::addTransparentObj( 
        Object obj
    ) {
        m_transparentObjects.emplace_back(std::move(obj));
    }

    void Scene::addMirrorObj(
        Object obj
    ) {
        std::pair<Object, smartCubemap> pair{std::move(obj), nullptr};
        m_mirrorObjects.emplace_back(std::move(pair)); // Cubemap is empty first
    }

    void Scene::addInstancedObj(
        Object obj,
        size_t numInstances
    ) {
        m_instancedObjects.emplace_back(std::move(obj), numInstances);
    }

    void Scene::addDirLight( 
        DirectionalLight dirLight
    ) {
        m_dirLights.push_back(std::make_pair(dirLight, nullptr));
    }

    void Scene::addPointLight( 
        PointLight pointLight
    ) {
        m_pointLights.push_back(std::make_pair(pointLight, nullptr));
    }

    void Scene::addSpotLight( 
        SpotLight spotLight
    ) {
        m_spotLights.push_back(std::make_pair(spotLight, nullptr));
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

    instancedObjsVec &Scene::getInstancedObjs(
    ) {
        return m_instancedObjects;
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