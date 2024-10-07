#include "minion_key_task.hpp"

MinionKeyTask::MinionKeyTask(MinionUtility::CommandKey key, uint32_t len, uint64_t from, char *chunk, UID uid)
        : m_task_data(key, len, from, chunk, uid)
{
}

MinionKeyTask::~MinionKeyTask()
{
    delete[] m_task_data.m_chunk;
    m_task_data.m_chunk = nullptr;
}

int MinionKeyTask::GetKey()
{
    return m_task_data.m_key;
}

MinionKeyTask::TaskData MinionKeyTask::GetData()
{
    return m_task_data;
}
