#include <unistd.h> //getpid
#include <chrono> //std::chrono::system_clock

#include "uid.hpp"

// Debug:
#include <iostream>
using namespace std;
void UID::Print()
{
	cout << "UID: " 
		 << " counter = " << m_counter
     	 << " pid = " << m_pid
		 << endl;
}

std::mutex UID::m_mutex;

UID::UID() :m_pid(getpid()), m_time(std::chrono::system_clock::now())
{
    static size_t count = 0;

	std::lock_guard<std::mutex> lock(m_mutex);
	m_counter = count++;
}

bool UID::operator==(const UID &other) const
{
    return (m_counter == other.m_counter && 
			m_pid == other.m_pid && 
			m_time == other.m_time);
}

char *UID::ToBuffer(char *message_buff_)
{
	*(size_t *)message_buff_ = m_counter;
	message_buff_ += sizeof(size_t);

	*(pid_t *)message_buff_ = m_pid;
	message_buff_ += sizeof(pid_t);

	*(std::chrono::system_clock::time_point *)message_buff_ = m_time;
	message_buff_ += sizeof(std::chrono::system_clock::time_point);

    return message_buff_;
}

char *UID::FromBuffer(char *message_buff_)
{
	m_counter = *(size_t *)message_buff_;
	message_buff_ += sizeof(size_t);

	m_pid = *(pid_t *)message_buff_;
	message_buff_ += sizeof(pid_t);

	m_time = *(std::chrono::system_clock::time_point *)message_buff_;
	message_buff_ += sizeof(std::chrono::system_clock::time_point);

    return message_buff_;
}

size_t UID::GetBufferLength() const
{
    return sizeof(size_t) + sizeof(pid_t) + sizeof(std::chrono::system_clock::time_point);
}
