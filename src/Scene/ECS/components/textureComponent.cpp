#include "Scene/ECS/components/textureComponent.h"

TextureComponent::TextureComponent(bool ispbr) {
	isPBR = ispbr;
	type = ComponentType::Texture;
}

TextureComponent::TextureComponent() {
	isPBR = true;
	type = ComponentType::Texture;
}

nlohmann::json TextureComponent::serialize() const
{
	return nlohmann::json();
}

TextureComponent::~TextureComponent() {

}
