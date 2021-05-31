#include "LevelSaver.hpp"

namespace OGL::E1 {

    void LevelSaver::save(
        std::string const &filePath,
        modelsTable const &modelsTable,
        Scene &scene
    ) {
        std::ofstream file(filePath);

        auto pathToIDTable = reverseModelsTable(modelsTable);

        nlohmann::json level;
        level["Models"] = modelsTableToJSON(modelsTable);

        BasicCamera const *camera = scene.getCamera().get();
        level["Camera"] = camera ? cameraToJSON(scene.getCamera().get()) : nlohmann::json{};

        nlohmann::json objects;
        objects["Normal"]      = objectsToJSON(scene.getNormalObjs(), pathToIDTable);
        objects["Transparent"] = objectsToJSON(scene.getTransparentObjs(), pathToIDTable);
        objects["Mirror"]      = objectsToJSON(scene.getMirrorObjs(), pathToIDTable);
        objects["Instanced"]   = objectsToJSON(scene.getInstancedObjs(), pathToIDTable);
        level["Objects"]       = objects;

        nlohmann::json lights;
        lights["Directional"] = lightsToJSON(scene.getDirLights());
        lights["Point"]       = lightsToJSON(scene.getPointLights());
        lights["Spot"]        = lightsToJSON(scene.getSpotLights());
        level["Lights"]       = lights;

        maybeSkybox const &skybox = scene.getSkybox();
        level["Skybox"] = skybox ? skyboxToJSON(skybox.value()) : nlohmann::json{};

        file << std::setw(4) << level;
    }

    nlohmann::json LevelSaver::modelsTableToJSON(
        modelsTable const &modelsTable
    ) {
        std::unordered_map<std::string, std::string> modelsPaths;
        for (auto &[key, val] : modelsTable) {
            modelsPaths[std::to_string(key)] = val->getFullPath();
        }
        return nlohmann::json(modelsPaths);
    }

    nlohmann::json LevelSaver::cameraToJSON(
        BasicCamera const *camera
    ) const {
        glm::vec3 pos  = camera->getPos();
        glm::vec3 fwd  = camera->getForward();
        float speed    = camera->m_speed;
        float fov      = camera->m_fov;
        float clipNear = camera->m_near;
        float clipFar  = camera->m_far;

        nlohmann::json json;
        json["Position"]   = { pos.x, pos.y, pos.z };
        json["Direction"]  = { fwd.x, fwd.y, fwd.z };
        json["Move Speed"] = speed;
        json["FoV"]        = fov;
        json["Clip Near"]  = clipNear;
        json["Clip Far"]   = clipFar;

        return json;
    }

    nlohmann::json LevelSaver::objectToJSON(
        Object const &obj,
        std::unordered_map<std::string, uint32_t> const &pathToIDTable
    ) const {
        glm::vec3 pos         = obj.getPosition();
        float scale           = obj.getScale();
        glm::vec3 eulerAngles = obj.getRotationAngles();
        auto tableIt          = pathToIDTable.find(obj.getModelPath());
        uint32_t modelID      = (tableIt != pathToIDTable.end() ? tableIt->second : -1);
        uint32_t objectID     = obj.getID();

        nlohmann::json json;
        json["Position"]  = { pos.x, pos.y, pos.z };
        json["Scale"]     = scale;
        json["Rotation"]  = { eulerAngles.x, eulerAngles.y, eulerAngles.z };
        json["Model ID"]  = modelID;
        json["Object ID"] = objectID;

        return json;
    }

    nlohmann::json LevelSaver::objectToJSON(
        Object const &obj,
        uint32_t numInstances,
        std::unordered_map<std::string, uint32_t> const &pathToIDTable
    ) const {
        nlohmann::json json = objectToJSON(obj, pathToIDTable);
        json["Instances"] = numInstances;
        return json;
    }

    nlohmann::json LevelSaver::objectsToJSON(
        objectsVec const &objs,
        std::unordered_map<std::string, uint32_t> const &pathToIDTable
    ) const {
        std::vector<nlohmann::json> vec(objs.size());
        for (size_t i = 0; i < objs.size(); ++i) {
            vec[i] = objectToJSON(objs[i], pathToIDTable);
        }
        return nlohmann::json(std::move(vec));
    }

    nlohmann::json LevelSaver::objectsToJSON(
        mirrorObjsVec const &objs,
        std::unordered_map<std::string, uint32_t> const &pathToIDTable
    ) const {
        std::vector<nlohmann::json> vec(objs.size());
        for (size_t i = 0; i < objs.size(); ++i) {
            vec[i] = objectToJSON(objs[i].first, pathToIDTable);
        }
        return nlohmann::json(std::move(vec));
    }

    nlohmann::json LevelSaver::objectsToJSON(
        instancedObjsVec const &objs,
        std::unordered_map<std::string, uint32_t> const &pathToIDTable
    ) const {
        std::vector<nlohmann::json> vec(objs.size());
        for (size_t i = 0; i < objs.size(); ++i) {
            vec[i] = objectToJSON(objs[i].first, objs[i].second, pathToIDTable);
        }
        return nlohmann::json(std::move(vec));
    }

    nlohmann::json LevelSaver::lightToJSON(
        DirectionalLight const &dirLight
    ) const {
        glm::vec3 dir   = dirLight.m_direction;
        glm::vec3 color = dirLight.m_color;

        nlohmann::json json;
        json["Direction"] = { dir.x, dir.y, dir.z };
        json["Color"]     = { color.x, color.y, color.z };
        return json;
    }

    nlohmann::json LevelSaver::lightsToJSON(
        dirLights const &lights
    ) const {
        std::vector<nlohmann::json> vec(lights.size());
        for (size_t i = 0; i < lights.size(); ++i) {
            vec[i] = lightToJSON(lights[i].first);
        }
        return nlohmann::json(std::move(vec));
    }

    nlohmann::json LevelSaver::lightToJSON(
        PointLight const &pointLight
    ) const {
        glm::vec3 pos           = pointLight.m_position;
        glm::vec3 color         = pointLight.m_color;
        float attenuationConst  = pointLight.m_attenuationConst;
        float attenuationLinear = pointLight.m_attenuationLinear;
        float attenuationQuad   = pointLight.m_attenuationQuadratic;

        nlohmann::json json;
        json["Position"]    = { pos.x, pos.y, pos.z };
        json["Color"]       = { color.x, color.y, color.z };
        json["Attenuation"] = { attenuationConst, attenuationLinear, attenuationQuad };
        return json;
    }

    nlohmann::json LevelSaver::lightsToJSON(
        pointLights const &lights
    ) const {
        std::vector<nlohmann::json> vec(lights.size());
        for (size_t i = 0; i < lights.size(); ++i) {
            vec[i] = lightToJSON(lights[i].first);
        }
        return nlohmann::json(std::move(vec));
    }

    nlohmann::json LevelSaver::lightToJSON(
        SpotLight const &spotLight
    ) const {
        glm::vec3 pos           = spotLight.m_position;
        glm::vec3 dir           = spotLight.m_direction;
        glm::vec3 color         = spotLight.m_color;
        float cutOffInner       = spotLight.m_cutOffAngle;
        float cutOffOuter       = spotLight.m_cutOffOuterAngle;
        float attenuationConst  = spotLight.m_attenuationConst;
        float attenuationLinear = spotLight.m_attenuationLinear;
        float attenuationQuad   = spotLight.m_attenuationQuadratic;

        nlohmann::json json;
        json["Position"]    = { pos.x, pos.y, pos.z };
        json["Direction"]   = { dir.x, dir.y, dir.x };
        json["Color"]       = { color.x, color.y, color.z };
        json["CutOff"]      = { cutOffInner, cutOffOuter };
        json["Attenuation"] = { attenuationConst, attenuationLinear, attenuationQuad };
        return json;
    }

    nlohmann::json LevelSaver::lightsToJSON(
        spotLights const &lights
    ) const {
        std::vector<nlohmann::json> vec(lights.size());
        for (size_t i = 0; i < lights.size(); ++i) {
            vec[i] = lightToJSON(lights[i].first);
        }
        return nlohmann::json(std::move(vec));
    }

    nlohmann::json LevelSaver::skyboxToJSON(
        Skybox const &skybox
    ) const {
        nlohmann::json json;
        json["Folder Path"]  = skybox.folderPath();
        json["Texture Unit"] = skybox.unit() - GL_TEXTURE0;
        return json;
    }

    std::unordered_map<std::string, uint32_t> LevelSaver::reverseModelsTable(
        modelsTable const &table
    ) const {
        std::unordered_map<std::string, uint32_t> reversed;
        for (auto &[key, val] : table) {
            reversed[val->getFullPath()] = key;
        }
        return reversed;
    }

} // OGL::E1