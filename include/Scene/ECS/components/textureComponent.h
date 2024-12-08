#pragma once
#include "Scene/ECS/component.h"


class TextureComponent : public Component {
    uint64_t textureIndex, normalIndex, armIndex, depthIndex;
    bool isPBR;
public:
    
    TextureComponent(bool ispbr);
    TextureComponent();

    uint64_t getColorTextureIndex() {
            return textureIndex;
    }
    uint64_t getNormalTextureIndex() {
         return normalIndex;
    }

    uint64_t getARMTextureIndex() {
        return armIndex;
    }

    uint64_t getDepthTextureIndex() {
        return depthIndex;
    }

    void setColorTextureIndex(uint64_t id) {
        textureIndex = id;
    }
    void setNormalTextureIndex(uint64_t id) {
        this->normalIndex = id;
    }

    void setARMTextureIndex(uint64_t id) {
        this->armIndex = id;
    }

    void setDepthTextureIndex(uint64_t id) {
        depthIndex = id;
    }

    bool* isPBRTexture() {
        return &isPBR;
    }
    nlohmann::json serialize() const override;
    ~TextureComponent();


};