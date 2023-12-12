#ifndef THREAD_TASK_EXECUTOR
#define THREAD_TASK_EXECUTOR

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

class ThreadTaskExecutor
{

public:

    ThreadTaskExecutor() : worker([this] { run(); })
    {

    }

    ~ThreadTaskExecutor()
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }

        cv.notify_all();

        worker.join();
    }

    void AddTask(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.push(std::move(task));
        }

        cv.notify_one();
    }

private:

    std::thread worker;
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<std::function<void()>> tasks;
    bool stop = false;

    void run()
    {
        while (true)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex);

                cv.wait(lock, [this]
                {
                    return stop || !tasks.empty();
                });

                if (stop && tasks.empty())
                    return;

                task = std::move(tasks.front());

                tasks.pop();
            }

            task();
        }
    }
};

#endif // !THREAD_TASK_EXECUTOR