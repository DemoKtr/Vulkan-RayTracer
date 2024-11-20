#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <future>
#include"View/egnine.h"



namespace TaskSystem {
    struct ThumbnailTask {
        uint32_t width;
        uint32_t height;

        ThumbnailTask(uint32_t w, uint32_t h) : width(w), height(h) {}
        void execute(GraphicsEngine* engine) {

        }
    };

    class ThumbnailTaskQueue {
        std::queue<ThumbnailTask> taskQueue;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stopWorker = false;

        void workerThread(GraphicsEngine* engine);


        void onModelAdded(GraphicsEngine* engine, uint32_t thumbnailWidth, uint32_t thumbnailHeight);


            
    };
}
/*
int main() {
    Engine engine;

    std::thread worker(workerThread, &engine);

    // Dodanie modelu symulacyjnie
    onModelAdded(&engine);
    onModelAdded(&engine);
    onModelAdded(&engine);
    onModelAdded(&engine);
    onModelAdded(&engine);
    onModelAdded(&engine);
    onModelAdded(&engine);

    // Po zakoñczeniu pracy
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopWorker = true;
    }

    condition.notify_one();
    while(true)
    std::cout << "1" << std::endl;

    worker.join();

    return 0;
}
*/