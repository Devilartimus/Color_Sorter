#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "ColoredObject.h"
#include "utils.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <array>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class ThreadManager {
public:
    ThreadManager();
    ~ThreadManager();

    void setRule(const std::string& rule_str);
    void startProcessing(const std::string& filename);
    void stopProcessing();
    const std::vector<ColoredObject>& getResult() const;
    bool isProcessing() const;

private:
    class SafeBuffer {
    public:
        void push(ColoredObject&& item);
        std::optional<ColoredObject> tryPop();
        void markComplete();
        bool isComplete() const;
        void reset();

    private:
        std::queue<ColoredObject> m_buffer;
        mutable std::mutex m_BufferMtx;
        std::condition_variable m_cv;
        bool m_is_complete = false;
    };

    void producerThread(const std::string& filename);
    void consumerThread();

    SafeBuffer m_buffer;
    std::array<Color, 3> m_rule;
    std::array<std::vector<ColoredObject>, 3> m_buckets;
    std::vector<ColoredObject> m_result;
    std::atomic<bool> m_is_processing{false};
    std::atomic<bool> m_stop_requested{false};
    std::thread m_producer_thread;
    std::thread m_consumer_thread;
};

#endif // THREADMANAGER_H
