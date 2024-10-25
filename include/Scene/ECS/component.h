#pragma once
#include <cstddef>
#include "components/componentType.h"
#include <string>
constexpr std::size_t MAX_COMPONENTS = 32;
struct Component {
    virtual ~Component() = default;
public:
    ComponentType type;
    // Metoda get dla type
    ComponentType getType() const {
        return type;
    }
    std::string getLabel() {
		std::string label;
		switch (this->getType()) {
		case ComponentType::Transform:
			label = "Transform";
			break;
		case ComponentType::Mesh:
			label = "Mesh";
			break;
		case ComponentType::Render:
			label = "Light";
			break;
		case ComponentType::Script:
			label = "Camera";
			break;
			// Dodaj inne typy komponentów wed³ug potrzeby
		default:
			label = "Unknown Component";
			break;
		}
		return label;
	}
};