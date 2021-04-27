#include "LevelLoader.hpp"

namespace OGL::E1 {

    void LevelLoader::load(
        int screenWidth,
        int screenHeight,
        std::string const &filePath,
        modelsTable &modelsTable,
        smartScenePtr &scenePtr
    ) {
        std::ifstream file(filePath);

        nlohmann::json levelJSON;
        file >> levelJSON;

        modelsTable = getModelsTable(levelJSON);
        
        float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
        scenePtr = factory<Scene>(getCamera(levelJSON, aspect));

        scenePtr->getNormalObjs()      = getNormalObjs(levelJSON, modelsTable);
        scenePtr->getTransparentObjs() = getTransparentObjs(levelJSON, modelsTable);
        scenePtr->getMirrorObjs()      = getMirrorObjs(levelJSON, modelsTable);
        scenePtr->getInstancedObjs()   = getInstancedObjs(levelJSON, modelsTable);

        scenePtr->getDirLights()   = getDirLights(levelJSON);
        scenePtr->getPointLights() = getPointLights(levelJSON);
        scenePtr->getSpotLights()  = getSpotLights(levelJSON);

        scenePtr->getSkybox() = getSkybox(levelJSON);
    }

    modelsTable LevelLoader::getModelsTable(
        nlohmann::json const &levelJSON
    ) const {
        std::unordered_map<std::string, std::string> fromJSON = levelJSON["Models"];
        modelsTable table;
        for (auto &[strKey, path] : fromJSON) {
            int key = std::stoi(strKey);
            table.emplace(key, Model{ path });
        }
        return table;
    }

    smartCamPtr LevelLoader::getCamera(
        nlohmann::json const &levelJSON,
        float aspect
    ) const {
        nlohmann::json camJSON;
        try {
            camJSON = levelJSON["Camera"];

            std::array<float, 3> pos = camJSON["Position"];
            std::array<float, 3> fwd = camJSON["Direction"];
            float moveSpeed = camJSON["Move Speed"];
            float fov = camJSON["FoV"];
            float clipNear = camJSON["Clip Near"];
            float clipFar = camJSON["Clip Far"];

            return factory<CameraFree>(
                glm::vec3{ pos[0], pos[1], pos[2] },
                glm::vec3{ fwd[0], fwd[1], fwd[2] },
                moveSpeed,
                fov,
                aspect,
                clipNear,
                clipFar
            );
        }
        catch (nlohmann::detail::type_error) {
            return nullptr;
        }
    }

    Object LevelLoader::getNormalObj(
        nlohmann::json const &objJSON,
        modelsTable &modelsTable
    ) const {
        uint32_t modelID         = objJSON["Model ID"];
        uint32_t objectID        = objJSON["Object ID"];
        std::array<float, 3> pos = objJSON["Position"];
        float scale              = objJSON["Scale"];
        std::array<float, 3> rot = objJSON["Rotation"];
        return Object{
            modelsTable.at(modelID),
            glm::vec3{pos[0], pos[1], pos[2]},
            scale,
            glm::vec3{rot[0], rot[1], rot[2]},
            objectID
        };
    }

    objectsVec LevelLoader::getNormalObjs(
        nlohmann::json const &levelJSON,
        modelsTable &modelsTable
    ) const {
        std::vector<nlohmann::json> objJSONs = levelJSON["Objects"]["Normal"];
        objectsVec objects;
        objects.reserve(objJSONs.size());
        for (nlohmann::json &objJSON : objJSONs) {
            objects.emplace_back(getNormalObj(objJSON, modelsTable));
        }
        return objects;
    }

    Object LevelLoader::getTransparentObj(
        nlohmann::json const &objJSON,
        modelsTable &modelsTable
    ) const {
        return getNormalObj(objJSON, modelsTable);
    }

    objectsVec LevelLoader::getTransparentObjs(
        nlohmann::json const &levelJSON,
        modelsTable &modelsTable
    ) const {
        std::vector<nlohmann::json> objJSONs = levelJSON["Objects"]["Transparent"];
        objectsVec objects;
        objects.reserve(objJSONs.size());
        for (nlohmann::json &objJSON : objJSONs) {
            objects.emplace_back(getTransparentObj(objJSON, modelsTable));
        }
        return objects;
    }

    std::pair<Object, smartCubemap> LevelLoader::getMirrorObj(
        nlohmann::json const &objJSON,
        modelsTable &modelsTable
    ) const {
        return std::make_pair(getNormalObj(objJSON, modelsTable), nullptr);
    }

    mirrorObjsVec LevelLoader::getMirrorObjs(
        nlohmann::json const &levelJSON,
        modelsTable &modelsTable
    ) const {
        std::vector<nlohmann::json> objJSONs = levelJSON["Objects"]["Mirror"];
        mirrorObjsVec objects;
        objects.reserve(objJSONs.size());
        for (nlohmann::json &objJSON : objJSONs) {
            objects.emplace_back(getMirrorObj(objJSON, modelsTable));
        }
        return objects;
    }

    std::pair<Object, uint32_t> LevelLoader::getInstancedObj(
        nlohmann::json const &objJSON,
        modelsTable &modelsTable
    ) const {
        uint32_t numInstances = objJSON["Instances"];
        return std::make_pair(getNormalObj(objJSON, modelsTable), numInstances);
    }

    instancedObjsVec LevelLoader::getInstancedObjs(
        nlohmann::json const &levelJSON,
        modelsTable &modelsTable
    ) const {
        std::vector<nlohmann::json> objJSONs = levelJSON["Objects"]["Instanced"];
        instancedObjsVec objects;
        objects.reserve(objJSONs.size());
        for (nlohmann::json &objJSON : objJSONs) {
            objects.emplace_back(getInstancedObj(objJSON, modelsTable));
        }
        return objects;
    }

    DirectionalLight LevelLoader::getDirLight(
        nlohmann::json const &lightJSON
    ) const {
        std::array<float, 3> dir = lightJSON["Direction"];
        std::array<float, 3> col = lightJSON["Color"];

        return DirectionalLight{
            glm::vec3{dir[0], dir[1], dir[2]},
            glm::vec3{col[0], col[1], col[2]}
        };
    }

    dirLights LevelLoader::getDirLights(
        nlohmann::json const &levelJSON
    ) const {
        std::vector<nlohmann::json> dirLightJSONs = levelJSON["Lights"]["Directional"];
        dirLights lights;
        lights.reserve(dirLightJSONs.size());
        for (nlohmann::json &lightJSON : dirLightJSONs) {
            lights.emplace_back(getDirLight(lightJSON), nullptr);
        }
        return lights;
    }

    PointLight LevelLoader::getPointLight(
        nlohmann::json const &lightJSON
    ) const {
        std::array<float, 3> pos = lightJSON["Position"];
        std::array<float, 3> col = lightJSON["Color"];
        std::array<float, 3> att = lightJSON["Attenuation"];

        return PointLight{
            glm::vec3{pos[0], pos[1], pos[2]},
            glm::vec3{col[0], col[1], col[2]},
            att[0], att[1], att[2]
        };
    }

    pointLights LevelLoader::getPointLights(
        nlohmann::json const &levelJSON
    ) const {
        std::vector<nlohmann::json> pointLightJSONs = levelJSON["Lights"]["Point"];
        pointLights lights;
        lights.reserve(pointLightJSONs.size());
        for (nlohmann::json &lightJSON : pointLightJSONs) {
            lights.emplace_back(getPointLight(lightJSON), nullptr);
        }
        return lights;
    }

    SpotLight LevelLoader::getSpotLight(
        nlohmann::json const &lightJSON
    ) const {
        std::array<float, 3> pos    = lightJSON["Position"];
        std::array<float, 3> dir    = lightJSON["Direction"];
        std::array<float, 3> col    = lightJSON["Color"];
        std::array<float, 2> cutOff = lightJSON["CutOff"];
        std::array<float, 3> att    = lightJSON["Attenuation"];

        return SpotLight{
            glm::vec3{pos[0], pos[1], pos[2]},
            glm::vec3{dir[0], dir[1], dir[2]},
            glm::vec3{col[0], col[1], col[2]},
            cutOff[0], cutOff[1],
            att[0], att[1], att[2]
        };
    }

    spotLights LevelLoader::getSpotLights(
        nlohmann::json const &levelJSON
    ) const {
        std::vector<nlohmann::json> spotLightJSONs = levelJSON["Lights"]["Spot"];
        spotLights lights;
        lights.reserve(spotLightJSONs.size());
        for (nlohmann::json &lightJSON : spotLightJSONs) {
            lights.emplace_back(getSpotLight(lightJSON), nullptr);
        }
        return lights;
    }

    smartSkybox LevelLoader::getSkybox(
        nlohmann::json const &levelJSON
    ) const {
        nlohmann::json skyboxJSON;
        try {
            skyboxJSON = levelJSON["Skybox"];

            return factory<Skybox>(
                skyboxJSON["Folder Path"],
                skyboxJSON["Texture Unit"] + GL_TEXTURE0
            );
        }
        catch (nlohmann::detail::type_error) {
            return nullptr;
        }
    }

}