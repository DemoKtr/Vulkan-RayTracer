#include "View/vkParticle/ParticleGenerator.h"
#include <MultithreatedSystems/SimdInstructions/simdRandom.h>
#include <chrono>


// Konstruktor
vkParticle::ParticleGenerator::ParticleGenerator(bool* ptr)
    : randomEngine((static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))), randomDist(0.0f, 1.0f),
    positionMin(0.0f), positionMax(1.0f), velocityMin(-1.0f), velocityMax(1.0f),
    accelerationMin(0.0f), accelerationMax(0.0f), sizeMin(1.0f), sizeMax(1.0f),
    colorMin(0.0f), colorMax(1.0f), lifetimeMin(2.0f), lifetimeMax(5.0f),
    rotationMin(0.0f), rotationMax(0.0f), rotationSpeedMin(0.0f), rotationSpeedMax(0.0f),
    textureIndex(0) {
    dirty_flag = ptr;
}

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

void vkParticle::ParticleGenerator::GenerateParticles() {
    
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
    /*
        size_t count = particles.size();
        size_t simdCount = count / 8; // Liczba cz¹steczek przetwarzanych w grupach po 4
        #pragma omp parallel for
        for (size_t i = 0; i < simdCount; ++i) {
            // SIMD dla pozycji
            __m256 posX = RandomSIMD8(positionMin.x, positionMax.x);
            __m256 posY = RandomSIMD8(positionMin.y, positionMax.y);
            __m256 posZ = RandomSIMD8(positionMin.z, positionMax.z);

            // SIMD dla prêdkoœci
            __m256 velX = RandomSIMD8(velocityMin.x, velocityMax.x);
            __m256 velY = RandomSIMD8(velocityMin.y, velocityMax.y);
            __m256 velZ = RandomSIMD8(velocityMin.z, velocityMax.z);

            // SIMD dla przyspieszenai
            __m256 accX = RandomSIMD8(accelerationMin.x, accelerationMax.x);
            __m256 accY = RandomSIMD8(accelerationMin.y, accelerationMax.y);
            __m256 accZ = RandomSIMD8(accelerationMin.z, accelerationMax.z);

            __m256 sizeX = RandomSIMD8(sizeMin.x, sizeMax.x);
            __m256 sizeY = RandomSIMD8(sizeMin.y, sizeMax.y);
            __m256 sizeZ = RandomSIMD8(sizeMin.z, sizeMax.z);

            // SIMD dla koloru
            __m256 colR = RandomSIMD8(colorMin.r, colorMax.r);
            __m256 colG = RandomSIMD8(colorMin.g, colorMax.g);
            __m256 colB = RandomSIMD8(colorMin.b, colorMax.b);

            __m256 lrsX = RandomSIMD8(lifetimeMin, lifetimeMax);
            __m256 lrsY = RandomSIMD8(lifetimeMin, lifetimeMax);
            __m256 lrsZ = RandomSIMD8(lifetimeMin, lifetimeMax);



            // Wpisz dane do cz¹steczek
            for (int j = 0; j < 8; ++j) {
                particles[i * 8 + j].position = glm::vec4(
                    reinterpret_cast<float*>(&posX)[j],
                    reinterpret_cast<float*>(&posY)[j],
                    reinterpret_cast<float*>(&posZ)[j],
                    1.0f
                );

                particles[i *8 + j].velocity = glm::vec4(
                    reinterpret_cast<float*>(&velX)[j],
                    reinterpret_cast<float*>(&velY)[j],
                    reinterpret_cast<float*>(&velZ)[j],
                    1.0f
                );

                particles[i * 8 + j].color = glm::vec4(
                    reinterpret_cast<float*>(&colR)[j],
                    reinterpret_cast<float*>(&colG)[j],
                    reinterpret_cast<float*>(&colB)[j],
                    1.0f
                );
            }
        }

        // Obs³uga pozosta³ych cz¹steczek (jeœli liczba cz¹steczek nie jest podzielna przez 4)
        for (size_t i = simdCount * 8; i < count; ++i) {
            particles[i].position = glm::vec4(
                Random(positionMin.x, positionMax.x),
                Random(positionMin.y, positionMax.y),
                Random(positionMin.z, positionMax.z),
                1.0f
            );
            particles[i].position = glm::vec4(
                Random(positionMin.x, positionMax.x),
                Random(positionMin.y, positionMax.y),
                Random(positionMin.z, positionMax.z),
                1.0f
            );
        }
        
    }
    */
  
}
std::vector<vkParticle::Particle>& vkParticle::ParticleGenerator::GetParticles()
{
    return particles;
}

void vkParticle::ParticleGenerator::GenerateParticles(std::vector<ParticleInit>& data) {
    MutexManager& mutexManager = MutexManager::getInstance();
   
    for (auto& particle : data) {

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

void vkParticle::ParticleGenerator::update() { 
        MutexManager& mutexManager = MutexManager::getInstance();
       
        particles.resize(particleCount, Particle());
        GenerateParticles();     
        *dirty_flag = false;     
        
           
}


