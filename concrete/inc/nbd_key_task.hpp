#ifndef ILRD_RD1556_KEY_TASK_HPP
#define ILRD_RD1556_KEY_TASK_HPP

#include <linux/nbd.h> //nbd_reply

#include "ikey_task.hpp"
#include "nbd.hpp"

class NBDKeyTask : public IKeyTask
{
public:
    struct key_task_data;

    NBDKeyTask(int request_type, uint32_t len, uint64_t from, char *chunk, nbd_reply reply);
    ~NBDKeyTask();
    int GetKey();
    key_task_data &GetData();
private:
    struct key_task_data
    {
        // Constructor for key_task_data
        key_task_data();
        key_task_data(int key, uint32_t len, uint64_t from, char *data, nbd_reply reply)
            : m_key(key), m_len(len), m_from(from), m_chunk(data), m_reply(reply) { }
            
        int m_key;
        uint32_t m_len;
        uint64_t m_from;
        char *m_chunk;
        nbd_reply m_reply;

    } m_key_task_data;
};

#endif //ILRD_RD1556_KEY_TASK_HPP