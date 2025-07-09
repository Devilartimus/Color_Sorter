#include "../include/ThreadManager.h"
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

ThreadManager::ThreadManager() = default;

ThreadManager::~ThreadManager() {
    stopProcessing();
}

void ThreadManager::setRule(const std::string& rule_str) {
    m_rule = parseRule(rule_str);
}

void ThreadManager::startProcessing(const std::string& filename) {
    if (m_is_processing) {
        std::cerr << "Processing is already running. Stop it first.\n";
        return;
    }

    // Сброс состояния системы
    m_is_processing = true;
    m_stop_requested = false;
    m_buckets = {};
    m_result.clear();
    m_buffer.reset();

    // Создание потоков только при начале обработки
    m_producer_thread = std::thread(&ThreadManager::producerThread, this, filename);
    m_consumer_thread = std::thread(&ThreadManager::consumerThread, this);

    std::cout << "Threads created and started\n";
}

void ThreadManager::stopProcessing() {
    if (!m_is_processing) return;

    m_stop_requested = true;

    // Ожидание завершения и уничтожение потоков
    if (m_producer_thread.joinable()) {
        m_producer_thread.join();
    }

    if (m_consumer_thread.joinable()) {
        m_consumer_thread.join();
    }

    m_is_processing = false;
    std::cout << "Threads stopped and destroyed\n";
}

const std::vector<ColoredObject>& ThreadManager::getResult() const {
    return m_result;
}

bool ThreadManager::isProcessing() const {
    return m_is_processing;
}

void ThreadManager::producerThread(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string line;
        while (!m_stop_requested && std::getline(file, line)) {
            try {
                ColoredObject obj = parseLine(line);
                m_buffer.push(std::move(obj));
            } catch (const std::invalid_argument& e) {
                std::cerr << "[WARNING] Skipping invalid line: " << e.what() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Producer: " << e.what() << "\n";
    }
    m_buffer.markComplete();
}

void ThreadManager::consumerThread() {
    while (!m_stop_requested || !m_buffer.isComplete()) {
        if (auto item = m_buffer.tryPop()) {
            int color_index = static_cast<int>(item->m_color);
            if (color_index >= 0 && color_index < 3) {
                m_buckets[color_index].push_back(std::move(*item));
            }
        }
    }

    // Финализация результатов
    for (Color c : m_rule) {
        int index = static_cast<int>(c);
        if (index >= 0 && index < 3) {
            auto& bucket = m_buckets[index];
            m_result.insert(m_result.end(), std::make_move_iterator(bucket.begin()), std::make_move_iterator(bucket.end()));
        }
    }
}

// Реализация методов SafeBuffer
void ThreadManager::SafeBuffer::push(ColoredObject&& item) {
    std::lock_guard<std::mutex> lock(m_BufferMtx);
    m_buffer.push(std::move(item));
    m_cv.notify_one();
}

std::optional<ColoredObject> ThreadManager::SafeBuffer::tryPop() {
    std::unique_lock<std::mutex> lock(m_BufferMtx);
    if (m_cv.wait_for(lock, 100ms, [this] {
            return !m_buffer.empty() || m_is_complete;
        })) {
        if (!m_buffer.empty()) {
            ColoredObject item = std::move(m_buffer.front());
            m_buffer.pop();
            return item;
        }
    }
    return std::nullopt;
}

void ThreadManager::SafeBuffer::markComplete() {
    std::lock_guard<std::mutex> lock(m_BufferMtx);
    m_is_complete = true;
    m_cv.notify_all();
}

bool ThreadManager::SafeBuffer::isComplete() const {
    std::lock_guard<std::mutex> lock(m_BufferMtx);
    return m_is_complete && m_buffer.empty();
}

void ThreadManager::SafeBuffer::reset() {
    std::lock_guard<std::mutex> lock(m_BufferMtx);
    m_buffer = std::queue<ColoredObject>();
    m_is_complete = false;
}
