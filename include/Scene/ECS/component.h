#pragma once
#include <cstddef>
#include "components/componentType.h"
#include <string>
#include <nlohmann/json.hpp>
constexpr std::size_t MAX_COMPONENTS = 32;

class Component {
    
public:
	virtual ~Component() = default;
    ComponentType type;
    // Metoda get dla type
    ComponentType getType() const {
        return type;
    }
    std::string getLabel() {
		std::string label;
        switch (this->getType()) {
            case ComponentType::Transform: label = "Transform"; break;
            case ComponentType::Mesh: label = "Mesh"; break;
            case ComponentType::Texture: label = "Texture"; break;
            case ComponentType::Script: label = "Script"; break;
            default: label = "Unknown Component"; break;
        }
		return label;
	}
    virtual nlohmann::json serialize() const = 0;

	//virtual std::vector<std::type_index> getRequiredComponents() const = 0;
};