#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <memory>
#include <type_traits>

// Szablon ThreadSafeResource
template <typename T>
class ThreadSafeResource {
    T resource;
    std::mutex resourceMutex;

public:
    void set(const T& value) {
        std::lock_guard<std::mutex> lock(resourceMutex);
        resource = value;
    }

    T get() {
        std::lock_guard<std::mutex> lock(resourceMutex);
        return resource;
    }
};

// Typ priorytetu zadania
enum class TaskPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    DESCRIPTORS = 3,
    RECORD_DRAW_COMMAND = 4
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

// Komparator do sortowania zadań
struct TaskComparator {
    bool operator()(const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) {
        return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority());
    }
};

// TaskManager
class TaskManager {
public:
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

    // Poprawiona metoda submitTask z bardziej ogólnym zliczaniem aktywnych zadań
    template<typename Func, typename... Args>
    void submitTask(TaskPriority priority, Func&& func, Args&&... args) {
        {
            std::lock_guard<std::mutex> lock(counterMutex);
            ++priorityTaskCount[priority];  // Zwiększ licznik dla danego priorytetu
        }

        auto task = std::make_shared<Task>(priority, [this, priority, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
            syncArgs(args);
            std::apply(func, std::move(args));

            // Zmniejsz licznik po wykonaniu zadania
            {
                std::lock_guard<std::mutex> lock(counterMutex);
                --priorityTaskCount[priority];
                if (priorityTaskCount[priority] == 0) {
                    priorityCondition.notify_all();  // Powiadom, jeśli wszystkie zadania zostały zakończone
                }
            }
            });

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(task);  // Dodaj zadanie do kolejki
        }
        queueCondition.notify_one();  // Powiadom, że zadanie zostało dodane
    }

    // Poprawiona metoda czekania na zadania o danym priorytecie
    void waitForPriorityTasks(TaskPriority expectedPriority) {
        std::unique_lock<std::mutex> lock(counterMutex);
        priorityCondition.wait(lock, [this, expectedPriority]() {
            return priorityTaskCount[expectedPriority] == 0;  // Czekaj na zakończenie wszystkich zadań o danym priorytecie
            });
    }


private:
    TaskManager() : running(false) {
        priorityTaskCount[TaskPriority::LOW] = 0;
        priorityTaskCount[TaskPriority::MEDIUM] = 0;
        priorityTaskCount[TaskPriority::HIGH] = 0;
        priorityTaskCount[TaskPriority::DESCRIPTORS] = 0;
        priorityTaskCount[TaskPriority::RECORD_DRAW_COMMAND] = 0;
    }

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
                task->execute();
            }
        }
    }

    template<typename T>
    void syncArgs(T&& arg) {
        if constexpr (std::is_base_of_v<ThreadSafeResource<typename std::remove_reference<T>::type>, T>) {
            std::lock_guard<std::mutex> lock(arg.resourceMutex);
        }
    }

    template<typename T, typename... Rest>
    void syncArgs(T&& arg, Rest&&... rest) {
        syncArgs(std::forward<T>(arg));
        syncArgs(std::forward<Rest>(rest)...);
    }

    std::vector<std::thread> threads;
    std::priority_queue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>, TaskComparator> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondition;

    std::atomic<bool> running;

    std::mutex counterMutex;
    std::condition_variable priorityCondition;
    std::unordered_map<TaskPriority, size_t> priorityTaskCount;  // Śledzenie liczby aktywnych zadań dla każdego priorytetu

    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
};

//EXAMPLE TO USE
/*
// Przykładowe zadanie
void sampleTask(ThreadSafeResource<int>& sharedResource, int taskId, int value) {
    std::cout << "Task " << taskId << " executing. Shared resource value: " << sharedResource.get() << "\n";
    sharedResource.set(sharedResource.get() + value);
    std::cout << "Task " << taskId << " updated shared resource value: " << sharedResource.get() << "\n";
}

int main() {
    // Inicjalizacja TaskManagera
    auto& taskManager = TaskManager::getInstance();
    taskManager.initialize(); // Domyślnie liczba wątków = liczbie logicznych rdzeni CPU

    // Tworzenie zasobu współdzielonego
    ThreadSafeResource<int> sharedResource;
    sharedResource.set(5);

    // Dodanie zadań do managera
    taskManager.submitTask(TaskPriority::HIGH, sampleTask, std::ref(sharedResource), 1, 3);
    taskManager.submitTask(TaskPriority::MEDIUM, sampleTask, std::ref(sharedResource), 2, 1);
    taskManager.submitTask(TaskPriority::LOW, sampleTask, std::ref(sharedResource), 3, 2);
    taskManager.submitTask(TaskPriority::HIGH, sampleTask, std::ref(sharedResource), 4, 6);
    taskManager.submitTask(TaskPriority::MEDIUM, sampleTask, std::ref(sharedResource), 5, 4);

    // Poczekaj na zakończenie wszystkich zadań
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Zakończenie pracy TaskManagera
    taskManager.shutdown();

    return 0;
}
*/