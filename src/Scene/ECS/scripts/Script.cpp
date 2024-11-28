#include "Scene/ECS/scripts/scriptLoader.h"


scripts::Script::Script(std::string libraryPath) {
    this->libraryPath = libraryPath;
    this->handle = this->loadLibrary();
}

scripts::Script::Script(const Script& script)
    : ecs(script.ecs),                      // Skopiowanie wskaünika (p≥ytka kopia)
    entity(script.entity),                // Kopiowanie wartoúci
    libraryPath(script.libraryPath),      // Kopiowanie std::string
    handle(nullptr) {                     // Domyúlnie ustaw handle jako nullptr
    std::cout << "Konstruktor kopiujπcy wywo≥any" << std::endl;
}

scripts::Script::Script()
{
}

scripts::Script::~Script() {
    this->closeLibrary();
}

void scripts::Script::setECSContext(ecs::ECS* ecs, ecs::Entity entity) {
    this->ecs = ecs;
    this->entity = entity;
}

void scripts::Script::execute_onStart_function() {
    auto onStart = (void (*)())getFunction("onStart");
    if (onStart) {
        onStart(); // Wywo≥anie funkcji
    }
}

void scripts::Script::execute_onUpdate_function(float deltaTime) {
    auto onUpdate = (void (*)(float))getFunction("onUpdate");
    if (onUpdate) {
        onUpdate(deltaTime); // Wywo≥anie funkcji
    }
}

void* scripts::Script::getFunction(std::string functionName)
{
#ifdef _WIN32
    void* func = (void*)GetProcAddress((HMODULE)handle, functionName.c_str());
    if (!func) {
        std::cerr << "Error finding function: " << GetLastError() << std::endl;
    }
    return func;
#else
    void* func = dlsym(handle, functionName);
    if (!func) {
        std::cerr << "Error finding function: " << dlerror() << std::endl;
    }
    return func;
#endif
}

void* scripts::Script::loadLibrary() {
#ifdef _WIN32
    HMODULE handle = LoadLibraryA(libraryPath.c_str());
    if (!handle) {
        std::cerr << "Error loading library: " << GetLastError() << std::endl;
    }
    return handle;
#else
    void* handle = dlopen(libraryPath, RTLD_NOW);
    if (!handle) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
    }
    return handle;
#endif
}

void scripts::Script::closeLibrary() {
#ifdef _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}


