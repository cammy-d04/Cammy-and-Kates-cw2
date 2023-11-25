#include "include/cache.h"
#include <stdlib.h>

// Cache statistics counters.
uint32_t cache_total_accesses;
uint32_t cache_hits;
uint32_t cache_misses;
uint32_t cache_read_accesses;
uint32_t cache_read_hits;
uint32_t cache_write_accesses;
uint32_t cache_write_hits;

// Input parameters to control the cache.
uint32_t cache_size;
uint32_t cache_associativity;
uint32_t cache_block_size;

block_t **cache;

//structure to store mapping between tag and lru_counter
typedef struct
{
    uint32_t tag;
    uint32_t lru_counter;
} tag_lru_mapping_t;

tag_lru_mapping_t *tag_counter_map;
//stores value of counter 
uint32_t global_lru_counter;


/*
 * Perform a read from the memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to read from memory.
 */
op_result_t read_from_cache(uint32_t pa) 
{
    uint32_t num_cache_lines = cache_size / cache_block_size;
    uint32_t num_sets = num_cache_lines / cache_associativity;
    uint32_t set_index = (pa / cache_block_size) % num_sets; // find target block ensure mapped to index within 0 to (num_sets-1)
    uint32_t tag = pa / (cache_block_size * num_sets);

    for (uint32_t i = 0; i < cache_associativity; i++) {
        // check if the valid bit is set and tag matches block
        if (cache[set_index][i].valid && cache[set_index][i].tag == tag) {
            // cache hit
            //increment stats
            cache_total_accesses++;
            cache_hits++;
            cache_read_accesses++;
            cache_read_hits++;
            

            // loop through lru counter structure and find matching tags, increment counter for that block
            for (uint32_t j = 0; j < cache_associativity; j++) {
                if (tag_counter_map[j].tag == cache[set_index][i].tag) {
                    tag_counter_map[j].lru_counter = global_lru_counter;
                    global_lru_counter++;
                    break;
                }
            }
            return HIT;
        }
    }

    // cache miss
    // increment stats
    cache_total_accesses++;
    cache_misses++;
    cache_read_accesses++;

    uint32_t lru_index = 0;
    uint32_t min_lru_counter = tag_counter_map[0].lru_counter;

    for (uint32_t i = 1; i < cache_associativity; i++) {
        // find least recently used cache line
        if (tag_counter_map[i].lru_counter < min_lru_counter) {
            min_lru_counter = tag_counter_map[i].lru_counter;
            lru_index = i;
        }
    }

    //r ead from memory and store in cache
    read_from_memory(pa);

    cache[set_index][lru_index].valid = 1;
    cache[set_index][lru_index].tag = tag;

    // update LRU counter for the accessed line
    tag_counter_map[lru_index].lru_counter = global_lru_counter;
    global_lru_counter++;

    return MISS;
}
/*
 * Perform a write from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */
op_result_t write_to_cache(uint32_t pa)
{
	return ERROR;
}

// Process the S parameter properly and initialize `cache_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_S(int opt, char *optarg)
{
	   
    int value = atoi(optarg); //convert optarg to integer

    //check if conversion was successful
    if (value <= 0 || value % 4 != 0) {
        printf("Invalid configuration\n");
        return -1;
    }

    //assign the value to cache_size
    cache_size = (uint32_t)value;

    return 0;
}

// Process the A parameter properly and initialize `cache_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_A(int opt, char *optarg)
{ 
    int value = atoi(optarg);// convert optarg to integer

    //check if conversion was successful
    if (value <= 0) {
        printf("Invalid configuration\n");
        return -1;
    }

    //assign the value to cache_associativity
    cache_associativity = (uint32_t)value;

    return 0;
}

// Process the B parameter properly and initialize `cache_block_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_B(int opt, char *optarg)
{
	 
    int value = atoi(optarg);//convert optarg to integer

    //check if conversion was successful
    if (value <= 0 || value % 4 != 0) {
        printf("Invalid configuration\n");
        return -1;
    }

    //assign the value to cache_block_size
    cache_block_size = (uint32_t)value;
    return 0;
}

// When verbose is true, print the details of each operation -- MISS or HIT.
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret)
{
    {
    if (verbose) {
        char operation_type = entry.accesstype == READ ? 'R' : 'W';
        printf("%c 0x%08x CACHE-%s\n", operation_type, entry.address, ret == HIT ? "HIT" : "MISS");
    }
    }

	if (ret == ERROR) {
		printf("This message should not be printed. Fix your code\n");
	}
}




//auxiliary is power of 2 func
int is_power_of_two(uint32_t num){
    
    if (num == 0)
        return 0;
    while (num != 1) {
        if (num % 2 != 0)
            return 0;
        num = num / 2;
    }
    return 1;
}

// Check if all the necessary paramaters for the cache are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_cache_parameters_valid()
{
	if (cache_size <= 0 || cache_associativity <=0 || cache_block_size <=0){
		printf("Invalid configuration\n"); //check that parameters are non negative
		return 1;
	}

	if(!is_power_of_two(cache_size)||!is_power_of_two(cache_block_size)){//check 
		printf("Invalid configuration\n");
		return 1;
	}

	if(cache_size % (cache_associativity * cache_block_size) !=0){
		printf("Invalid configuration");
		return 1;
	}

	if(cache_size < cache_block_size){
		printf("Invalid configuration\n");
		return 1;
	}

	else {
		return 0;
	}
}
