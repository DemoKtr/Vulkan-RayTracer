#include "Scene/sceneObjectFlagBits.h"

void scene::updateComponent(SceneObjectFlagBits flag, uint8_t& componentMask) {
	// Ustawianie odpowiedniego bitu w masce (1 oznacza zaktualizowany komponent)
	componentMask |= (1 << static_cast<uint8_t>(flag));
}

void scene::resetComponent(SceneObjectFlagBits flag, uint8_t& componentMask) {
	// Zerowanie odpowiedniego bitu w masce (0 oznacza, ¿e komponent nie zosta³ zaktualizowany)
	componentMask &= ~(1 << static_cast<uint8_t>(flag));
}

bool scene::isComponentUpdated(SceneObjectFlagBits flag, uint8_t& componentMask) {
	// Sprawdzenie, czy dany komponent zosta³ zaktualizowany
	return componentMask & (1 << static_cast<uint8_t>(flag));
}

void scene::setAllComponentsUpdated(uint8_t& componentMask) {
	// Ustawienie wszystkich bitów na 1 (wszystkie komponenty zaktualizowane)
	componentMask = 0xFF;  // Ustawienie wszystkich bitów na 1 (11111111)
}
