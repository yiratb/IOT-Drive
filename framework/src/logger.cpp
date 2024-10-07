/****************************************************                                                        
 *  Written by: Yirat Romano                         
 *                                                  
 *  Reviewed by:                              
 *                                                  
 *  Completed on - 26/06/23                         
 *                                                  
 ****************************************************/

#include <iostream>
#include <functional> //std::bind
#include <iomanip> //put_time

#include "../inc/logger.hpp" //logger

#define RESET "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"

using namespace std;

Logger::Logger(const std::string &path_) : m_is_active(true)
            
{
    m_file.open(path_);

    // Debug:
    cout << "Logger() 1\n";

    if (!m_file.is_open()) 
    {
        // Debug:
        cout << "Logger() 1.5\n";

        throw std::runtime_error("Failed to open the m_file\n");
    }

    cout << "Logger() 2\n";    

    m_file << "\tDate\t\tType\t\tMsg\n";

    m_thread = std::thread(std::bind(&Logger::ThreadWork, this));
}

Logger::~Logger()
{
    while (!m_wqueue.IsEmpty()){}

    m_is_active = false;
    
    m_thread.join();
    m_file.close();
}

void Logger::Log(const std::string &msg_, MsgType type_)
{
    LogData log_data = {std::chrono::system_clock::now(), type_, msg_};

    m_wqueue.Push(log_data);
}

void Logger::ThreadWork()
{
    LogData log_data;
    std::chrono::milliseconds timeout(20);

    while (m_is_active)
    {
        if (m_wqueue.Pop(log_data, timeout))
        {
            std::time_t time = std::chrono::system_clock::to_time_t(log_data.m_time);
            m_file << std::put_time(std::localtime(&time), "%d-%m-%Y %H:%M:%S") << '\t';
            switch (log_data.m_type)
            {
                case INFO:
                    m_file << "[INFO] \t";
                    break;
                case DEBUG:
                    m_file << YELLOW << "[DEBUG] " << RESET;
                    break;
                case ERROR:
                    m_file << RED << "[ERROR] " << RESET;
                    break;
                default:
                    break;
            }

            m_file << "\t" << log_data.m_msg << std::endl;
        }
    }
}
