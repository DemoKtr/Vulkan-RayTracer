#include <mutex>
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

class MutexManager {
public:
    // Pobierz instancjê singletona
    static MutexManager& getInstance() {
        static MutexManager instance;
        return instance;
    }

    // Dodanie nowego mutexa
    void addMutex(const std::string& name) {
        std::lock_guard<std::mutex> lock(globalMutex);
        if (mutexMap.find(name) == mutexMap.end()) {
            mutexMap[name] = std::make_unique<std::mutex>();
        }
    }

    // Zablokowanie mutexa
    void lock(const std::string& name) {
        std::unique_lock<std::mutex> lock(globalMutex);
        auto it = mutexMap.find(name);
        if (it != mutexMap.end()) {
            lock.unlock(); // Odblokowujemy globalny mutex przed zablokowaniem docelowego
            it->second->lock();
        } else {
            throw std::runtime_error("Mutex not found: " + name);
        }
    }

    // Odblokowanie mutexa
    void unlock(const std::string& name) {
        std::unique_lock<std::mutex> lock(globalMutex);
        auto it = mutexMap.find(name);
        if (it != mutexMap.end()) {
            lock.unlock(); // Odblokowujemy globalny mutex przed odblokowaniem docelowego
            it->second->unlock();
        } else {
            throw std::runtime_error("Mutex not found: " + name);
        }
    }

private:
    MutexManager() = default;
    ~MutexManager() = default;

    std::mutex globalMutex; // Mutex chroni¹cy dostêp do mapy mutexów
    std::unordered_map<std::string, std::unique_ptr<std::mutex>> mutexMap; // Mapa mutexów
};
