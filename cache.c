#include "cache.h"
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

/*
 * Perform a read from the memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to read from memory.
 */
int read_from_memory(uint32_t pa) 
{
	return 0;
}

/*
 * Perform a write from the cache to memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to write to memory.
 */
int write_to_memory(uint32_t pa)
{
	return 0;
}

/*
 *********************************************************
  Please edit the below functions for Part 1. 
  You are allowed to add more functions that may help you
  with your implementation. However, please do not add
  them in any file. All your new code should be in cache.c
  file below this line. 
 *********************************************************
*/

/*
 * Initialize the cache depending on the input parameters S, A, and B 
 * and the statistics counter. The cache is declared in as extern in 
 * include/cache.h file.
 */
void initialize_cache()
{
	uint32_t num_cache_lines = cache_size / cache_block_size;
	uint32_t num_sets = num_cache_lines / cache_associativity;

	cache = (block_t**)calloc(num_sets, sizeof(block_t*)); // allocate 2d array of pointers

	for (uint32_t set = 0; set < num_sets; set++) {
    	cache[set] = (block_t *)calloc(cache_associativity, sizeof(block_t));
	}
	
	return; 
}

/*
 * Free the allocated memory for the cache to avoid memory leaks.
 */
void free_cache()
{	uint32_t num_cache_lines = cache_size / cache_block_size;
	uint32_t num_sets = num_cache_lines / cache_associativity;

	// free memory for each set
    for (uint32_t i = 0; i < num_sets; ++i) {
        free(cache[i]);
    }
    // free memory for the cache
    free(cache);
	return;
}

// Print cache statistics.
void print_cache_statistics()
{
	printf("total accesses: %d\n", cache_total_accesses);
	printf("hits: %d\n", cache_hits);
	printf("misses: %d\n", cache_misses);
	printf("total reads: %d\n", cache_read_accesses);
	printf("read hits: %d\n", cache_read_hits);
	printf("total writes: %d\n", cache_write_accesses);
	printf("write hits: %d\n", cache_write_hits);
}

/*
 * Perform a read from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */

op_result_t read_from_cache(uint32_t pa) 
{
	uint32_t num_cache_lines = cache_size / cache_block_size;
	uint32_t num_sets = num_cache_lines / cache_associativity;
	uint32_t set_index = (pa / cache_block_size)% num_sets;
	uint32_t tag = pa / (cache_block_size * num_sets);

	


	return ERROR;
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
        printf("Invalid S parameter: Cache size must be a positive integer and a multiple of 4.\n");
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
        printf("Associativity must be a positive integer.\n");
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
        printf("Block size must be a positive integer and a multiple of 4.\n");
        return -1;
    }

    //assign the value to cache_block_size
    cache_block_size = (uint32_t)value;
}

// When verbose is true, print the details of each operation -- MISS or HIT.
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret)
{
	if (ret == ERROR) {
		printf("This message should not be printed. Fix your code\n");
	}
}

// Check if all the necessary paramaters for the cache are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_cache_parameters_valid()
{
	if (cache_size <= 0 || cache_associativity <=0 || cache_block_size <=0){
		printf("Size, associativity and block size must be non negative.\n");
		return -1;
	}

	if(!is_power_of_two(cache_size)||!is_power_of_two(cache_block_size)){
		printf("Size and block size must be powers of two.\n");
		return -1;
	}

	if(cache_size % (cache_associativity * cache_block_size) !=0){
		printf("Size must be divisible by (associativity * block size).\n");
		return -1;
	}

	if(cache_size < cache_block_size){
		printf("Cache block size must be <= cache size.\n");
		return -1;
	}

	else {
		return 0;
	}
}

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