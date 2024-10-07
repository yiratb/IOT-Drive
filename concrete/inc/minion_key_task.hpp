#ifndef ILRD_RD1556_MINION_KEY_TASK_HPP
#define ILRD_RD1556_MINION_KEY_TASK_HPP

#include <cstring> //strncpy

#include "minion_utility.hpp"
#include "ikey_task.hpp"
#include "uid.hpp"

class MinionKeyTask : public IKeyTask
{
public:
    struct TaskData;

    MinionKeyTask(MinionUtility::CommandKey key, uint32_t len, uint64_t from, char *chunk, UID uid);
    ~MinionKeyTask();
    int GetKey();
    TaskData GetData();
private:
    struct TaskData
    {
        // Constructor for task_data
        TaskData();
        TaskData(MinionUtility::CommandKey key, uint32_t len, uint64_t from, char *data, UID uid)
            : m_key(key), m_len(len), m_from(from), m_chunk(new char[m_len]), m_uid(uid)
            {
                if (nullptr != data)
                {
                    // Debug:
                    memcpy(m_chunk, data, m_len);
                    //strncpy(m_chunk, data, m_len);
                }
                
            }
        
        MinionUtility::CommandKey m_key;
        uint32_t m_len;
        uint64_t m_from;
        char *m_chunk;
        UID m_uid;

    } m_task_data;
};

#endif //ILRD_RD1556_MINION_KEY_TASK_HPP