#include "View/vkParticle/ParticleGenerator.h"


// Konstruktor
vkParticle::ParticleGenerator::ParticleGenerator(bool* dirtyFlag)
    : randomEngine((static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))), randomDist(0.0f, 1.0f),
    positionMin(0.0f), positionMax(1.0f), velocityMin(-1.0f), velocityMax(1.0f),
    accelerationMin(0.0f), accelerationMax(0.0f), sizeMin(1.0f), sizeMax(1.0f),
    colorMin(0.0f), colorMax(1.0f), lifetimeMin(1.0f), lifetimeMax(1.0f),
    rotationMin(0.0f), rotationMax(0.0f), rotationSpeedMin(0.0f), rotationSpeedMax(0.0f),
    textureIndex(0), dirty_flag(dirtyFlag) {}

// Random float generator
float vkParticle::ParticleGenerator::Random(float min, float max) {
    return randomDist(randomEngine) * (max - min) + min;
}

// Setters and Getters


void vkParticle::ParticleGenerator::GetPositionRange(glm::vec4& min, glm::vec4& max) const {
    min = positionMin;
    max = positionMax;
  
}



void vkParticle::ParticleGenerator::GetVelocityRange(glm::vec4& min, glm::vec4& max) const {
    min = velocityMin;
    max = velocityMax;
   
}



void vkParticle::ParticleGenerator::GetAccelerationRange(glm::vec4& min, glm::vec4& max) const {
    min = accelerationMin;
    max = accelerationMax;
    
}


void vkParticle::ParticleGenerator::GetSizeRange(glm::vec4& min, glm::vec4& max) const {
    min = sizeMin;
    max = sizeMax;
   
}



void vkParticle::ParticleGenerator::GetColorRange(glm::vec4& min, glm::vec4& max) const {
    min = colorMin;
    max = colorMax;
}



void vkParticle::ParticleGenerator::GetLifetimeRange(float& min, float& max) const {
    min = lifetimeMin;
    max = lifetimeMax;
}



void vkParticle::ParticleGenerator::GetRotationRange(float& min, float& max) const {
    min = rotationMin;
    max = rotationMax;
}



void vkParticle::ParticleGenerator::GetRotationSpeedRange(float& min, float& max) const {
    min = rotationSpeedMin;
    max = rotationSpeedMax;
}



uint32_t vkParticle::ParticleGenerator::GetTextureIndex() const {
    return textureIndex;
}

// Generate particles
void vkParticle::ParticleGenerator::GenerateParticles()  {
    for (auto& particle : particles) {
        particle.position = glm::vec4(
            Random(positionMin.x, positionMax.x),
            Random(positionMin.y, positionMax.y),
            Random(positionMin.z, positionMax.z),
            1.0f // czwarta wspó³rzêdna (a) zawsze ustawiona na 1.0f
        );
        
        particle.velocity = glm::vec4(
            Random(velocityMin.x, velocityMax.x),
            Random(velocityMin.y, velocityMax.y),
            Random(velocityMin.z, velocityMax.z),
            1.0f
        );

        particle.acceleration = glm::vec4(
            Random(accelerationMin.x, accelerationMax.x),
            Random(accelerationMin.y, accelerationMax.y),
            Random(accelerationMin.z, accelerationMax.z),
            1.0f
        );

        particle.size = glm::vec4(
            Random(sizeMin.x, sizeMax.x),
            Random(sizeMin.y, sizeMax.y),
            Random(sizeMin.z, sizeMax.z),
            1.0f
        );

        particle.color = glm::vec4(
            Random(colorMin.r, colorMax.r),
            Random(colorMin.g, colorMax.g),
            Random(colorMin.b, colorMax.b),
            1.0f // Alfa (przezroczystoœæ) ustawiona na 1.0f
        );

        particle.lifeRotationRSpeed = glm::vec4(
            Random(lifetimeMin, lifetimeMax),      // lifetime
            Random(rotationMin, rotationMax),     // rotation
            Random(rotationSpeedMin, rotationSpeedMax), // rotation speed
            0.0f // lub inna domyœlna wartoœæ, jeœli ta wspó³rzêdna nie jest u¿ywana
        );
        
    }
}

std::vector<vkParticle::Particle>& vkParticle::ParticleGenerator::GetParticles()
{
    return particles;
}



void vkParticle::ParticleGenerator::update() { 
        particles.resize(particleCount,Particle());
        GenerateParticles();
           
}
