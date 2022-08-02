#include "ThreadPool.h"

void ThreadPool::Start(int count_thread) {
    threads.resize(count_thread);
    for (int i = 0; i < count_thread; i++) {
        threads.at(i) = std::thread(std::bind(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
            });
            if (should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::QueueJob(const std::function<void()> &job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        while (!jobs.empty())
            jobs.pop();
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}