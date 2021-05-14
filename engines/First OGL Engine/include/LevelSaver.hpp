/**
* @file LevelSaver.hpp
*/

#ifndef OGL_E1_LEVELSAVER_HPP
#define OGL_E1_LEVELSAVER_HPP

#include "Scene.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
#include <iomanip>

namespace OGL::E1 {

    /**
     * @brief Class to convert Scene data into json file
    */
    class LevelSaver {
     public:
        /**
         * @brief Converts Scene data into json file
         * @param filePath Path to json file
         * @param modelsTable Hashmap: Key - Model's ID, Value - Model object 
         * @param scene Scene to be converted into json
        */
        void save(
            std::string const &filePath,
            modelsTable const &modelsTable,
            Scene &scene
        );

     protected:
         /**
          * @brief Converts Hashmap of Models into json object
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object 
          * @return json object with data from Hashmap of Models
         */
         nlohmann::json modelsTableToJSON(
             modelsTable const &modelsTable
         );

         /**
          * @brief Converts Scene's camera into json object
          * @param camera Scene's camera
          * @return json object with camera's data
         */
         nlohmann::json cameraToJSON(
             BasicCamera const *camera
         ) const;

         /**
          * @brief Converts Scene's Object into json object
          * @param obj Scene's Object
          * @param pathToIDTable HashMap: Key - Model's path, Value - Model's ID
          * @return json object with data from Scene Object
         */
         nlohmann::json objectToJSON(
             Object const &obj,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         /**
          * @brief Converts Scene's Object into json object
          * @param obj Scene's Object
          * @param numInstances number of Object's instances
          * @param pathToIDTable HashMap: Key - Model's path, Value - Model's ID
          * @return json object with data from Scene Object
         */
         nlohmann::json objectToJSON(
             Object const &obj,
             uint32_t numInstances,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         /**
          * @brief Converts container of Objects into json array
          * @param objs Container of Objects
          * @param pathToIDTable HashMap: Key - Model's path, Value - Model's ID
          * @return json array with data from Objects in container
         */
         nlohmann::json objectsToJSON(
             objectsVec const &objs,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         /**
          * @brief Converts container of Objects into json array
          * @param objs Container of Objects
          * @param pathToIDTable HashMap: Key - Model's path, Value - Model's ID
          * @return json array with data from Objects in container
         */
         nlohmann::json objectsToJSON(
             mirrorObjsVec const &objs,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         /**
          * @brief Converts container of Objects into json array
          * @param objs Container of Objects
          * @param pathToIDTable HashMap: Key - Model's path, Value - Model's ID
          * @return json array with data from Objects in container
         */
         nlohmann::json objectsToJSON(
             instancedObjsVec const &objs,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         /**
          * @brief Converts DirectionalLight into json object
          * @param dirLight Scene's DirectionalLight
          * @return json object with data from DirectionalLight
         */
         nlohmann::json lightToJSON(
             DirectionalLight const &dirLight
         ) const;

         /**
          * @brief Converts container of DirectionalLights into json array
          * @param lights Container of DirectionalLights
          * @return json array with data from container of DirectionalLights
         */
         nlohmann::json lightsToJSON(
             dirLights const &lights
         ) const;

         /**
          * @brief Converts PointLight into json object
          * @param pointLight Scene's PointLight
          * @return json object with data from PointLight
         */
         nlohmann::json lightToJSON(
             PointLight const &pointLight
         ) const;

         /**
          * @brief Converts container of PointLight into json array
          * @param lights Container of PointLight
          * @return json array with data from container of PointLight
         */
         nlohmann::json lightsToJSON(
             pointLights const &lights
         ) const;

         /**
          * @brief Converts SpotLight into json object
          * @param spotLight Scene's SpotLight
          * @return json object with data from SpotLight
         */
         nlohmann::json lightToJSON(
             SpotLight const &spotLight
         ) const;

         /**
          * @brief Converts container of PointLight into json array
          * @param lights Container of PointLight
          * @return json array with data from container of PointLight
         */
         nlohmann::json lightsToJSON(
             spotLights const &lights
         ) const;

         /**
          * @brief Converts Scene's Skybox into json object
          * @param skybox Scene's Skybox
          * @return json object with data from Skybox
         */
         nlohmann::json skyboxToJSON(
             Skybox const &skybox
         ) const;

         /**
          * @brief Create utility hashmap to get Models' IDs from Objects
          * @param table Hashmap: Key - Model's ID, Value - Model object
          * @return HashMap: Key - Model's path, Value - Model's ID
         */
         std::unordered_map<std::string, uint32_t> reverseModelsTable(
             modelsTable const &table
         ) const;
    };

} // OGL::E1

#endif // OGL_E1_LEVELSAVER_HPP
