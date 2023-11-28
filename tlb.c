#include "tlb.h"
#include <stdlib.h>

// Input parameters to control the tlb.
uint32_t tlb_entries;
uint32_t tlb_associativity;

// TLB statistics counters.
uint32_t tlb_total_accesses;
uint32_t tlb_hits;
uint32_t tlb_misses;

tlb_entry_t** tlb;

// Check if all the necessary paramaters for the tlb are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_tlb_parameters_valid()
{
    if (tlb_entries < 2) {
        printf("Invalid configuration\n");
        return -1;
    }

    // check that tlb_associativity >=1
    if (tlb_associativity < 1) {
        printf("Invalid configuration\n");
        return -1;
    }
    return 0;
}

/*
 * Initialize the "tlb" depending on the input parameters T and L. 
 * The "tlb" is declared in as extern in include/tlb.h file.
 */
void initialize_tlb()
{
    uint32_t total_tlb_entries = tlb_entries * tlb_associativity;

	tlb = (tlb_entry_t**)total_tlb_entries, sizeof(tlb_entry_t*); // allocate memory space to tlb


	for (uint32_t i = 0; i < total_tlb_entries; i++)
    { //allocate memory foeach entry
    	tlb[i] = (tlb_entry_t*)calloc(tlb_associativity, sizeof(tlb_entry_t));
	}
	return; 
}

// Process the T parameter properly and initialize `tlb_entries`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_T(int opt, char *optarg)
{
    int value = atoi(optarg);
    if (value < 2) {
        printf("Invalid configuration\n");
        return -1;
    }
    tlb_entries = (uint32_t)value;
    return 0;
}

// Process the A parameter properly and initialize `tlb_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_L(int opt, char *optarg)
    { 
    int value = atoi(optarg);// convert optarg to integer

    // check if conversion was successful
    if (value <= 0) {
        printf("Invalid configuration\n");
        return -1;
    }

    // assign the value to tlb_associativity
    tlb_associativity = (uint32_t)value;

    return 0;
}
// Check if the tlb hit or miss.
// Extract the VPN from the address and use it.
// Keep associativity in mind while searching.
int check_tlb(uint32_t address)
    //return -1 if the entry is missing or valid bit is 0 aka tlb miss
    //return PPN if the entry is valid and TAG matches aka tlb hit
    {
    // extract vpn from the address
    uint32_t vpn = (address/4096) & 0xFFFFF;
    uint32_t set_index = vpn % tlb_entries;

    if (tlb_associativity == 1){
        //direct mapped
        // iterate through TLB entries
        for (uint32_t i = 0; i < tlb_entries; i++) {
            // Check if the entry is valid and VPN matches
            if (tlb[i]->valid && tlb[i]->VPN == vpn) {
               // TLB hit
               return tlb[i]->PPN;
            }
        }
    }else{
        //set associative
        // calculate the entry index
        for (uint32_t i = 0; i < tlb_associativity; i++){
            uint32_t entry_index = set_index * tlb_associativity + i;
            tlb_entry_t *current_entry = tlb[entry_index];

            // Check if the entry is valid and VPN matches
            if (current_entry->valid && current_entry->VPN == vpn) {
            // TLB hit
                return current_entry->PPN;
            }
        }
    }
 // TLB miss
    return -1;      
}


void set_dirty_bit_in_tlb(uint32_t address){
    //set the dirty bit of the entry to 1
}

// LRU replacement policy has to be implemented.
void insert_or_update_tlb_entry(uint32_t address, uint32_t PPN){
    // if the entry is free, insert the entry
    // if the entry is not free, identify the victim entry and replace it
    //set PPN for VPN in tlb
    //set valid bit in tlb
}

// print pt entries as per the spec
void print_tlb_entries(){
    //print the tlb entries
    printf("\nTLB Entries (Valid-Bit Dirty-Bit VPN PPN)\n");
}

// print tlb statistics as per the spec
void print_tlb_statistics(){
    //print the tlb statistics
    printf("\n* TLB Statistics *\n");
    printf("total accesses: %d\n", tlb_total_accesses);
    printf("hits: %d\n", tlb_hits);
    printf("misses: %d\n", tlb_misses);
}
