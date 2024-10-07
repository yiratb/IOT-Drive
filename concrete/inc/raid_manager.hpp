#ifndef ILRD_RD1556_RAID_MANAGER_HPP
#define ILRD_RD1556_RAID_MANAGER_HPP

#include <memory> // std::shared_ptr
#include <vector> //std::vector

#define ALLOWED_SINGLETON
#include "singleton.hpp" //Singleton
#undef ALLOWED_SINGLETON

#include "minion_proxy.hpp" // IMinionProxy

class RaidManager
{
public:
    using p_minion_proxy = std::pair<MinionProxy*, uint64_t>;
    using p_minion_proxy_pair = std::pair<p_minion_proxy, p_minion_proxy>;

    ~RaidManager();
    RaidManager(const RaidManager& other) = delete;
    RaidManager& operator=(const RaidManager& other) = delete;

    p_minion_proxy_pair GetMinionAndOffsetWrite(uint64_t offset);
    p_minion_proxy GetMinionAndOffsetRead(uint64_t offset);

private:
	explicit RaidManager();
    friend Singleton<RaidManager>;

	std::vector<MinionProxy*> m_minion_proxies;

	// Each minion has 8M disk space. 
	// 4M are used for data (this allows testing wit one minion only. NBD needs at least 4M), and 4M are used for backup of the next
    // minion’s data.
	const size_t MINION_DISK_SPACE;// Half of total 8M

};

#endif //ILRD_RD1556_RAID_MANAGER_HPP