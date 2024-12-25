#include "fileOperations/serializations.h"
#include <Scene/Objects/PrefabManager.h>

nlohmann::json fileOperations::serialize(Scene* scene) {

    nlohmann::json json;

    // Serializacja korzenia sceny

    if (scene->root) {
        
        json["root"] = serialize(*scene->root); // Wywo³uje funkcjê serializuj¹c¹ SceneObject
        
    }
    else {
      
        json["root"] = nullptr;
    }

    // Serializacja ECS
    if (scene->ecs) {
        
        json["ecs"] = serialize(scene->ecs); // Wywo³uje funkcjê serializuj¹c¹ ECS
       
    }
    else {
   
        json["ecs"] = nullptr;
    
    }
    return json;

}

nlohmann::json fileOperations::serialize(SceneObject obj) {
    nlohmann::json json;

    // Serializacja nazwy
    json["name"] = obj.getName();

    // Serializacja aktywnoœci
    json["isActive"] = obj.isActive;

    // Serializacja ID ECS
    json["id"] = obj.id; // Jeœli `id` to z³o¿ona struktura, dodaj odpowiedni¹ serializacjê

    // Serializacja dzieci
    json["children"] = nlohmann::json::array();
    for (auto* child : obj.children) {
        if (child) {
            json["children"].push_back(serialize(*child));
        }
    }

    // Serializacja rodzica
    json["parent"] = obj.parent ? obj.parent->getName() : "";


    return json;
}

nlohmann::json fileOperations::serialize(ecs::ECS* ecs) {
    nlohmann::json json;

    // Serializacja masek komponentów
    nlohmann::json entityMasksJson = nlohmann::json::object();
    for (const auto& [entity, mask] : ecs->entityMasks) {
        entityMasksJson[std::to_string(entity)] = mask.to_string();
    }
    json["entityMasks"] = entityMasksJson;

    // Serializacja komponentów
    nlohmann::json componentsJson = nlohmann::json::object();
    for (const auto& [typeIndex, entityMap] : ecs->components) {
        std::string typeName = typeIndex.name(); // Nazwa typu komponentu
        nlohmann::json entityComponentsJson = nlohmann::json::object();

        for (const auto [entity, componentVector] : entityMap) {
            nlohmann::json componentArray = nlohmann::json::array();
            for (const auto component : componentVector) {
                if (component) {
                    componentArray.push_back(component->serialize()); // Wymaga metody `toJson` w klasie `Component`
                }
            }
            entityComponentsJson[std::to_string(entity)] = componentArray;
        }

        componentsJson[typeName] = entityComponentsJson;
    }
    json["components"] = componentsJson;

    return json;
}

void fileOperations::saveToFile(const std::string& filepath, Scene* scene) {
    
    // Utwórz pe³n¹ œcie¿kê do pliku
    std::string fullPath = filepath;

    try {

        // Serializacja sceny do JSON-a
        nlohmann::json sceneJson = fileOperations::serialize(scene);

        // Zapis JSON-a do pliku
        std::ofstream outFile(fullPath);
        if (!outFile.is_open()) {
            throw std::ios_base::failure("Nie mo¿na otworzyæ pliku: " + fullPath);
        }
        
        outFile << sceneJson.dump(4); // Zapisujemy JSON w formacie czytelnym (indenty = 4)
        outFile.close();

        std::cout << "Scene saved sucess: " << fullPath << std::endl;
    }
    
    catch (const std::exception& e) {
        std::cerr << "B³¹d podczas zapisywania sceny: " << e.what() << std::endl;
    }

}

nlohmann::json fileOperations::loadFromFile(const std::string& filepath) {

	return nlohmann::json();
}
