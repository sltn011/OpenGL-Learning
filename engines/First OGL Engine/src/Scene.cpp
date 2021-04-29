#include "Scene.hpp"

namespace OGL::E1 {

    Scene::Scene( 
        smartCamPtr &&camera
    ) : m_camera{ std::move(camera) } {
    }

    Object &Scene::addNormalObj(
        Object obj
    ) {
        return m_normalObjects.emplace_back(std::move(obj));
    }

    Object &Scene::addTransparentObj(
        Object obj
    ) {
        return m_transparentObjects.emplace_back(std::move(obj));
    }

    std::pair<Object, maybeCubemap> &Scene::addMirrorObj(
        Object obj
    ) {
        std::pair<Object, maybeCubemap> pair{ std::move(obj), std::nullopt };
        return m_mirrorObjects.emplace_back(std::move(pair)); // Cubemap is empty first
    }

    std::pair<Object, size_t> &Scene::addInstancedObj(
        Object obj,
        size_t numInstances
    ) {
        return m_instancedObjects.emplace_back(std::move(obj), numInstances);
    }

    std::pair<DirectionalLight, maybeShadowMap> &Scene::addDirLight(
        DirectionalLight dirLight
    ) {
       return m_dirLights.emplace_back(dirLight, std::nullopt);
    }

    std::pair<PointLight, maybeShadowCubemap> &Scene::addPointLight(
        PointLight pointLight
    ) {
        return m_pointLights.emplace_back(pointLight, std::nullopt);
    }

    std::pair<SpotLight, maybeShadowMap> &Scene::addSpotLight(
        SpotLight spotLight
    ) {
       return m_spotLights.emplace_back(spotLight, std::nullopt);
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
        Skybox &&skybox
    ) {
        m_skybox = std::move(skybox);
    }

    maybeSkybox &Scene::getSkybox(
    ) {
        return m_skybox;
    }

    smartCamPtr &Scene::getCamera(
    ) {
        return m_camera;
    }

} // OGL::E1