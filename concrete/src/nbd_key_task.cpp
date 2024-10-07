#include "nbd_key_task.hpp"

NBDKeyTask::NBDKeyTask(int request_type, uint32_t len, uint64_t from, char *chunk, nbd_reply reply)
:m_key_task_data(request_type, len, from, chunk, reply)
{
}

NBDKeyTask::~NBDKeyTask()
{
    delete[] m_key_task_data.m_chunk;
}

int NBDKeyTask::GetKey()
{
    return m_key_task_data.m_key;
}

NBDKeyTask::key_task_data &NBDKeyTask::GetData()
{
    return m_key_task_data;
}