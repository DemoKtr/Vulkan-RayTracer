#pragma once
#include "Scene/ECS/component.h"


class TextureComponent : public Component {
	int textureIndex, normalIndex, armIndex, depthIndex;
    bool isPBR;
public:

    TextureComponent(bool ispbr);
    TextureComponent();

    int getColorTextureIndex() {
            return textureIndex;
    }
    int getNormalTextureIndex() {
         return normalIndex;
    }

    int getARMTextureIndex() {
        return armIndex;
    }

    int getDepthTextureIndex() {
        return depthIndex;
    }

    void setColorTextureIndex(int id) {
        textureIndex = id;
    }
    void setNormalTextureIndex(int id) {
        this->normalIndex = id;
    }

    void setARMTextureIndex(int id) {
        this->armIndex = id;
    }

    void setDepthTextureIndex(int id) {
        depthIndex = id;
    }

    bool* isPBRTexture() {
        return &isPBR;
    }

    ~TextureComponent();


};