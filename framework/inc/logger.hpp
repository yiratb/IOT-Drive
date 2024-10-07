/****************************************************                                                        
 *  Written by: Yirat Romano                         
 *                                                  
 *  Reviewed by:                              
 *                                                  
 *  Completed on - 26/06/23                         
 *                                                  
 ****************************************************/
#ifndef ILRD_RD1556_LOGGER_HPP
#define ILRD_RD1556_LOGGER_HPP

#include <string> //std::string
#include <thread> //std::thread
#include <chrono> //std::chrono
#include <fstream> //std::ofstream

#define ALLOWED_SINGLETON
#include "singleton.hpp" //singelton
#undef ALLOWED_SINGLETON
#include "wqueue.hpp" //WQueue
#include "pqueue.hpp" //PriorityQueue

class Logger
{
public:
    enum MsgType
    {
        INFO,
        DEBUG,
        ERROR
    };
    ~Logger();
    Logger(const Logger& other_) = delete;
    Logger& operator=(const Logger& other_) = delete;
    void Log(const std::string& msg_, MsgType type_);
private:
    struct LogData
    {
        std::chrono::time_point<std::chrono::system_clock> m_time; 
        MsgType m_type;
        std::string m_msg;
    };

    struct Compare
    {
        bool operator()(const LogData& ld1, const LogData& ld2)
        {
            return ld1.m_time > ld2.m_time;
        }
    };

    explicit Logger(const std::string& path_);
    friend Singleton<Logger, const std::string&>;
    
    void ThreadWork();
    WQueue<LogData, PriorityQueue<LogData, std::vector<LogData>, Compare> > m_wqueue;

    bool m_is_active;
    std::thread m_thread;
    std::ofstream m_file;
};

#endif //ILRD_RD1556_LOGGER_HPP

/*
VLG:
vlg --fair-sched=yes ./logger.out
*/