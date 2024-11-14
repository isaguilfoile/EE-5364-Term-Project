#include "cache.h"

/*
*   LLC Early Core Invalidation (ECI), on miss, update both L1 and L2 caches so the missed way is the MRU
*   Prior to evicting the LRU in the L2 cache, replace the new LRU in the L1 cache with the current LRU
*   Then prepare the new LRU for the ECI on the next cache miss  
*/

// initialize replacement state
void CACHE::eci_llc_initialize_replacement()
{

}

// find replacement victim
uint32_t CACHE::eci_llc_find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU
    return eci_lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type); 
}

// called on every cache hit and cache fill
void CACHE::eci_llc_update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    string TYPE_NAME;
    if (type == LOAD)
        TYPE_NAME = "LOAD";
    else if (type == RFO)
        TYPE_NAME = "RFO";
    else if (type == PREFETCH)
        TYPE_NAME = "PF";
    else if (type == WRITEBACK)
        TYPE_NAME = "WB";
    else
        assert(0);

    if (hit)
        TYPE_NAME += "_HIT";
    else
        TYPE_NAME += "_MISS";

    if ((type == WRITEBACK) && ip)
        assert(0);

    // uncomment this line to see the LLC accesses
    // cout << "CPU: " << cpu << "  LLC " << setw(9) << TYPE_NAME << " set: " << setw(5) << set << " way: " << setw(2) << way;
    // cout << hex << " paddr: " << setw(12) << paddr << " ip: " << setw(8) << ip << " victim_addr: " << victim_addr << dec << endl;

    // baseline LRU
    if (hit && (type == WRITEBACK)) // writeback hit does not update LRU state
        return;

    eci_victim_back_invalidate(set, way);
    return eci_lru_update(set, way);
}

// send LRU of LLC to LRU of L1C and then evict LRU in LLC
void CACHE::eci_victim_back_invalidate(uint32_t set, uint32_t way, uint32_t cpu){
    uint64_t lruAddress = block[set][way].address;
    eci_insert_at_lru(cpu, lruAddress);
}

void CACHE::eci_llc_replacement_final_stats()
{

}
