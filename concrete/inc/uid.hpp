#ifndef ILRD_RD1556_UID_HPP
#define ILRD_RD1556_UID_HPP

#include <mutex>

class UID
{
public:
    explicit UID();
    ~UID() = default;
    UID(const UID& other) = default;
    UID& operator=(const UID& other) = default;
    bool operator==(const UID& other) const;
    char *ToBuffer(char *message_buff_);
    char *FromBuffer(char *message_buff_);
    size_t GetBufferLength() const;

    struct HashFunction
    {
        std::size_t operator()(const UID& uid) const
        {
            std::size_t h1 = std::hash<std::size_t>{}(uid.m_counter);
            std::size_t h2 = std::hash<pid_t>{}(uid.m_pid);

            auto time_since_epoch = uid.m_time.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count();
            std::size_t h3 = std::hash<std::int64_t>{}(millis);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    // Debug:
    void Print();
private:
    size_t m_counter;
    pid_t m_pid;
    std::chrono::system_clock::time_point m_time;
    static std::mutex m_mutex;
};

#endif //ILRD_RD1556_UID_HPP