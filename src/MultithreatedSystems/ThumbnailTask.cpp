#include "MultithreatedSystems/MiniatureTask.h"

void TaskSystem::ThumbnailTaskQueue::workerThread(GraphicsEngine* engine) {
    while (true) {
        ThumbnailTask task(0, 0);

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !taskQueue.empty() || stopWorker; });

            if (stopWorker && taskQueue.empty())
                return;

            task = std::move(taskQueue.front());
            taskQueue.pop();
        }

        // Zamiast bezpoœredniego wywo³ania `task.execute(engine)`
        // Wywo³anie renderowania w nowym w¹tku asynchronicznie
        std::async(std::launch::async, &ThumbnailTask::execute, std::move(task), engine);
    }
}

void TaskSystem::ThumbnailTaskQueue::onModelAdded(GraphicsEngine* engine, uint32_t thumbnailWidth, uint32_t thumbnailHeight) {
    ThumbnailTask newTask(thumbnailWidth, thumbnailHeight);

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(std::move(newTask));
    }

    condition.notify_one();
}

