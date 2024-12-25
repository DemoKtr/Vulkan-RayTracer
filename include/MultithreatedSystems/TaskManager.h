#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <memory>

enum class TaskPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
};

class Task {
public:
    Task(TaskPriority priority, std::function<void()> func)
        : priority(priority), taskFunc(std::move(func)) {}

    TaskPriority getPriority() const { return priority; }

    void execute() { taskFunc(); }

private:
    TaskPriority priority;
    std::function<void()> taskFunc;
};

struct TaskComparator {
    bool operator()(const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) {
        return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority());
    }
};

class TaskManager {
public:
    // Singleton getter
    static TaskManager& getInstance() {
        static TaskManager instance;
        return instance;
    }

    void initialize(size_t threadCount = 0) {
        if (threadCount == 0) {
            threadCount = std::thread::hardware_concurrency();
        }

        running = true;
        for (size_t i = 0; i < threadCount; ++i) {
            threads.emplace_back(&TaskManager::workerThread, this);
        }
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running = false;
        }
        queueCondition.notify_all();
        for (std::thread& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void submitTask(TaskPriority priority, std::function<void()> taskFunc) {
        auto task = std::make_shared<Task>(priority, std::move(taskFunc));
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(task);

            // Zwiększ licznik aktywnych zadań, jeśli priorytet jest HIGH lub MEDIUM
            if (priority == TaskPriority::HIGH || priority == TaskPriority::MEDIUM) {
                highAndMediumActiveTasks.fetch_add(1, std::memory_order_relaxed);
            }
        }
        queueCondition.notify_one();
    }

    // Czekaj na zakończenie wszystkich zadań HIGH i MEDIUM
    void waitForHighAndMediumTasks() {
        std::unique_lock<std::mutex> lock(waitMutex);
        waitCondition.wait(lock, [this]() {
            return highAndMediumActiveTasks.load(std::memory_order_relaxed) == 0;
            });
    }

private:
    TaskManager() : running(false), highAndMediumActiveTasks(0) {}

    void workerThread() {
        while (running) {
            std::shared_ptr<Task> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCondition.wait(lock, [this]() {
                    return !taskQueue.empty() || !running;
                    });

                if (!running && taskQueue.empty()) return;

                task = taskQueue.top();
                taskQueue.pop();
            }

            if (task) {
                // Wykonaj zadanie
                task->execute();

                // Zmniejsz licznik aktywnych zadań HIGH i MEDIUM, jeśli dotyczy
                if (task->getPriority() == TaskPriority::HIGH || task->getPriority() == TaskPriority::MEDIUM) {
                    if (highAndMediumActiveTasks.fetch_sub(1, std::memory_order_relaxed) == 1) {
                        std::lock_guard<std::mutex> lock(waitMutex);
                        waitCondition.notify_all();
                    }
                }
            }
        }
    }

    // Synchronizacja kolejki zadań
    std::condition_variable queueCondition;
    std::mutex queueMutex;

    // Synchronizacja czekania na zadania HIGH i MEDIUM
    std::condition_variable waitCondition;
    std::mutex waitMutex;
    std::atomic<int> highAndMediumActiveTasks;

    // Wątki i kolejka zadań
    std::vector<std::thread> threads;
    std::priority_queue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>, TaskComparator> taskQueue;

    std::atomic<bool> running;

    // Disable copy and assignment
    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
};




// Funkcje symulujące różne zadania
void highPriorityTask() {
    std::cout << "High priority task running on thread: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Symulacja pracy
    std::cout << "End : " << std::this_thread::get_id() << "\n";
}

void mediumPriorityTask() {
    std::cout << "Medium priority task running on thread: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Symulacja pracy
    std::cout << "End : " << std::this_thread::get_id() << "\n";
}

void lowPriorityTask() {
    std::cout << "Low priority task running on thread: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Symulacja pracy
    std::cout << "End : " << std::this_thread::get_id() << "\n";
}

/*
int main() {
    // Inicjalizacja TaskManagera
    auto& taskManager = TaskManager::getInstance();
    taskManager.initialize();

    // Dodaj zadania o różnych priorytetach
    taskManager.submitTask(TaskPriority::HIGH, highPriorityTask);
    taskManager.submitTask(TaskPriority::MEDIUM, mediumPriorityTask);
    taskManager.submitTask(TaskPriority::LOW, lowPriorityTask);
    taskManager.submitTask(TaskPriority::HIGH, highPriorityTask);
    taskManager.submitTask(TaskPriority::HIGH, highPriorityTask);
    
    taskManager.submitTask(TaskPriority::LOW, lowPriorityTask);
    taskManager.submitTask(TaskPriority::LOW, lowPriorityTask);
    taskManager.submitTask(TaskPriority::LOW, lowPriorityTask);
    taskManager.submitTask(TaskPriority::LOW, lowPriorityTask);
    taskManager.submitTask(TaskPriority::LOW, lowPriorityTask);
    taskManager.submitTask(TaskPriority::HIGH, highPriorityTask);

    // Główny wątek czeka na zakończenie wszystkich zadań HIGH i MEDIUM
    std::cout << "Waiting for high and medium priority tasks to complete...\n";
    taskManager.waitForHighAndMediumTasks();
    std::cout << "High and medium priority tasks completed.\n";

    // Zakończenie pracy TaskManagera
    taskManager.shutdown();

    std::cout << "All tasks completed. Exiting...\n";
    return 0;
}
*/