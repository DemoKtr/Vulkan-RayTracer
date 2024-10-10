#pragma once
#include "Scene/ECS/ecs.h"

namespace ecs {
	class ECS;  // Forward declaration zamiast pe�nego nag��wka
	class ECSSystem {
	public:
		virtual void update(ECS& ecs) = 0; // Wirtualna metoda do implementacji logiki dla systemu
	};
}