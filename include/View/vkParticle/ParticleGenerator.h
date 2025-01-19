#pragma once
#include "View/vkParticle/Particle.h"
#include <vector>
#include <random>
#include <immintrin.h> // SIMD (SSE/AVX)
namespace vkParticle {
	class ParticleGenerator {
    private:
        size_t particleCount =1500;
        std::vector<Particle> particles;
        bool* dirty_flag;

        glm::vec4 positionMin;
        glm::vec4 positionMax;
        glm::vec4 velocityMin;
        glm::vec4 velocityMax;
        glm::vec4 accelerationMin;
        glm::vec4 accelerationMax;
        glm::vec4 sizeMin;
        glm::vec4 sizeMax;
        glm::vec4 colorMin;
        glm::vec4 colorMax;
        float lifetimeMin;
        float lifetimeMax;
        float rotationMin;
        float rotationMax;
        float rotationSpeedMin;
        float rotationSpeedMax;
        uint32_t textureIndex;

        // Random generator
        std::mt19937 randomEngine;
        std::uniform_real_distribution<float> randomDist;

        // Helper function to generate random float in range [min, max]
        float Random(float min, float max);

    public:
        ParticleGenerator(bool* dirtyFlag);

        // Setters and getters

        void GetPositionRange(glm::vec4& min, glm::vec4& max) const;


        void GetVelocityRange(glm::vec4& min, glm::vec4& max) const;


        void GetAccelerationRange(glm::vec4& min, glm::vec4& max) const;


        void GetSizeRange(glm::vec4& min, glm::vec4& max) const;


        void GetColorRange(glm::vec4& min, glm::vec4& max) const;


        void GetLifetimeRange(float& min, float& max) const;


        void GetRotationRange(float& min, float& max) const;


        void GetRotationSpeedRange(float& min, float& max) const;


        uint32_t GetTextureIndex() const;

        // Generate particles
        void GenerateParticles();

        std::vector<Particle>& GetParticles();

        void update();


        template <typename T>
        bool UpdateValue(T& currentValue, const T& newValue, bool& flag) {
            if (currentValue != newValue) {
                currentValue = newValue;
                flag = true;
                return true;
            }
            return false;
        }
	};
}