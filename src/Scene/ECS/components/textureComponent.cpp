#include "Scene/ECS/components/textureComponent.h"

TextureComponent::TextureComponent(bool ispbr) {
	isPBR = ispbr;
	type = ComponentType::Texture;
}

TextureComponent::TextureComponent() {
	isPBR = true;
	type = ComponentType::Texture;
}

nlohmann::json TextureComponent::serialize() const {
    nlohmann::json json;

    // Serializacja pola modelIndex
    json["textureIndex"] = textureIndex;
    json["normalIndex"] = normalIndex;
    json["armIndex"] = armIndex;
    json["depthIndex"] = depthIndex;
    json["isPBR"] = isPBR;

    return json;
}

TextureComponent::~TextureComponent() {

}
