#include "cache.h"


/*
*   LLC Early Core Invalidation (ECI), in the L1 cache, on miss in L1 it acts normally, but in most cases, both current
*   members of the cache (if the cache is 2 members) will be evicted, and replaced with members of the L2 cache
*/
uint32_t CACHE::find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU replacement policy for other caches 
    return lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type); 
}

void CACHE::update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    if (type == WRITEBACK) {
        if (hit) // wrietback hit does not update LRU state
            return;
    }

    return lru_update(set, way);
}
//never used? why
uint32_t CACHE::lru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way=0; way<NUM_WAY; way++) {
        if (block[set][way].valid == false) {

            DP ( if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    // LRU victim
    if (way == NUM_WAY) {
        for (way=0; way<NUM_WAY; way++) {
            if (block[set][way].lru == NUM_WAY-1) {

                DP ( if (warmup_complete[cpu]) {
                cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                cout << dec << " lru: " << block[set][way].lru << endl; });

                break;
            }
        }
    }

    if (way == NUM_WAY) {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}

void CACHE::lru_update(uint32_t set, uint32_t way)
{
    // update lru replacement state
    for (uint32_t i=0; i<NUM_WAY; i++) {
        if (block[set][i].lru < block[set][way].lru) {
            block[set][i].lru++;
        }
    }
    block[set][way].lru = 0; // promote to the MRU position
}

void CACHE::insert_at_lru(uint32_t cpu, uint64_t address){
    uint32_t set = (address >> LOG2_BLOCK_SIZE) & (NUM_SET - 1);
    uint64_t tag = address >> (LOG2_BLOCK_SIZE);

    uint32_t way;
    bool foundInL1 = false;

    for(way = 0; way < NUM_WAY; way++){
        if (block[set][way].valid && block[set][way].tag == tag){
            foundInL1 = true;
            break;
        }
    }
    // if the value is in the L1 cache, just place it into the LRU position
    if(foundInL1){
        uint32_t currLRU = block[set][way].lru;
            // update lru replacement state
        for (uint32_t i=0; i<NUM_WAY; i++) {
            if (block[set][i].lru < currLRU) {
                block[set][i].lru++;
            }
        }
        block[set][way].lru = NUM_WAY-1; // promote to the LRU position
        return;
    }
    //not found, must insert, look for invalid block first
    for(way = 0; way < NUM_WAY; way++){
        if(!block[set][way].valid){
            break;
        }
    }

    //if not found, evict the lru
    if (way == NUM_WAY){
        for (way = 0; way < NUM_WAY; way++){
            if(block[set][way].lru == NUM_WAY-1)
                break;
        }
    }
    //insert the block into the selected way
    block[set][way].valid = true;
    block[set][way].tag = tag;
    block[set][way].address = address;

    //update the counters
    for (uint32_t i = 0; i < NUM_WAY; i++) {
        if (block[set][i].lru < block[set][way].lru) {
            block[set][i].lru++;
        }
    }
    block[set][way].lru = NUM_WAY - 1; // block is now lru
}

void CACHE::replacement_final_stats()
{

}

#ifdef NO_CRC2_COMPILE
void InitReplacementState()
{
    
}

uint32_t GetVictimInSet (uint32_t cpu, uint32_t set, const BLOCK *current_set, uint64_t PC, uint64_t paddr, uint32_t type)
{
    return 0;
}

void UpdateReplacementState (uint32_t cpu, uint32_t set, uint32_t way, uint64_t paddr, uint64_t PC, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    
}

void PrintStats_Heartbeat()
{
    
}

void PrintStats()
{

}
#endif
