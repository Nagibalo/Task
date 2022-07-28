#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>
#include "TCPSocket.h"

class ThreadPool {
public:
    void Start(int count_thread);
    void QueueJob(const std::function<void()> &job);
    void Stop();

private:
    void ThreadLoop();
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue< std::function<void()> > jobs;
};
#endif // THREADPOOL_H
