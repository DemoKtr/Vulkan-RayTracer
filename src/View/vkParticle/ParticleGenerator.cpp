#include "View/vkParticle/ParticleGenerator.h"


// Konstruktor
vkParticle::ParticleGenerator::ParticleGenerator(bool* dirtyFlag)
    : randomEngine(std::random_device{}()), randomDist(0.0f, 1.0f),
    positionMin(0.0f), positionMax(0.0f), velocityMin(0.0f), velocityMax(0.0f),
    accelerationMin(0.0f), accelerationMax(0.0f), sizeMin(1.0f), sizeMax(1.0f),
    colorMin(1.0f), colorMax(1.0f), lifetimeMin(1.0f), lifetimeMax(1.0f),
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
        particle.position = glm::mix(positionMin, positionMax, Random(0.0f, 1.0f));
        particle.velocity = glm::mix(velocityMin, velocityMax, Random(0.0f, 1.0f));
        particle.acceleration = glm::mix(accelerationMin, accelerationMax, Random(0.0f, 1.0f));
        particle.size = glm::mix(sizeMin, sizeMax, Random(0.0f, 1.0f));
        particle.color = glm::mix(colorMin, colorMax, Random(0.0f, 1.0f));
        particle.lifetime = Random(lifetimeMin, lifetimeMax);
        particle.rotation = Random(rotationMin, rotationMax);
        particle.rotationSpeed = Random(rotationSpeedMin, rotationSpeedMax);
        particle.textureIndex = textureIndex;
    }
}



void vkParticle::ParticleGenerator::update() { 
        particles.resize(particleCount,Particle());
        GenerateParticles();
           
}
