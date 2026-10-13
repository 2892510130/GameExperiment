#pragma once

#include <mutex>

namespace Utils
{

template<typename Type>
class Synchronised {
public:
    Type getAtomic() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

    void setAtomic(const Type& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
    }

    // remember lock first
    Type& getData() {
        return m_data;
    }

    void lock() {
        m_mutex.lock();
    }

    void unlock() {
        m_mutex.unlock();
    }

    void operator=(const Synchronised<Type>& v) = delete; // no copy
private:
    mutable std::mutex m_mutex;
    Type m_data;
};

} // namespace Utils