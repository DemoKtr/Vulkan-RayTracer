#pragma once
#include <vector>
#include <iostream>
#include "Memory/MemoryPool.h"
namespace UI {


    template <typename T>
    class UIElementManager {
    private:
        std::vector<T*> elements;
        memory::BlockMemoryPool::MemoryPool pool;
        size_t poolOffset = 0;

    public:
        explicit UIElementManager(size_t count)
            : pool(sizeof(T)* count) {}

        template <typename... Args>
        T* createElement(Args&&... args) {
            
            T* element = pool.AllocateObject<T>(std::forward<Args>(args)...);
            elements.push_back(element);
            return element;
        }

        

        void updateAll(std::vector<glm::vec4>& PosSize, size_t& count) {
   
            for (auto& element : elements) {
                element->Update(PosSize,count++);
            }
        }

        void removeElement(T* element) {
            // Usuwamy element z listy
            auto it = std::find(elements.begin(), elements.end(), element);
            if (it != elements.end()) {
                // Zwalniamy pamiêæ elementu
                pool.DeallocateObject(*it);
                elements.erase(it);  // Usuwamy wskaŸnik z listy
            }
        }

        ~UIElementManager() {
            for (auto element : elements) {
                pool.DeallocateObject(element);
            }
        }
    };
    /* how to use
    class UIButton {
    public:
        void render() const {
            std::cout << "Rendering UIButton\n";
        }
    };

    class UIProgressBar {
    public:
        void render() const {
            std::cout << "Rendering UIProgressBar\n";
        }
    };
    
   
    int main() {
        UIElementManager<UIButton> buttonManager;
        UIElementManager<UIProgressBar> progressBarManager;

        UIButton* button = buttonManager.createElement();
        UIProgressBar* progressBar = progressBarManager.createElement();

        buttonManager.renderAll();
        progressBarManager.renderAll();

        return 0;
    }
     */
}