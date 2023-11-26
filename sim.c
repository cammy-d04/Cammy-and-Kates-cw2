/*
 ********************************************************
  Please do not edit anything in this file.
 ********************************************************
*/

#include <stdio.h>  
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/cache.h"

// Initialize the system depending on the input parameters.
void initialize()
{
	initialize_cache();
}

// Free the allocated memory for a graceful shutdown and to prevent memory leaks.
void free_memory()
{
	free_cache();
}

// Print system-wide statistics.
void print_statistics()
{
	print_cache_statistics();
}

// Print information when verbose is true.
void handle_verbose(memory_access_entry_t entry, op_result_t ret)
{
	handle_cache_verbose(entry, ret);
}

// Check if all input parameters are provided and valid.
int check_parameters_valid()
{
	return check_cache_parameters_valid();
}

int main(int argc, char *argv[])
{
	int opt;
	trace_file = NULL;
	FILE *trace_fp;
	opterr = 0;
	memory_access_entry_t entry;
	uint32_t pa = 0;
	op_result_t ret;
	int r = 0;

	/* 
	 * This is just an example to show how to use getopt. You will need to do a
     * lot more to get your code correct. Try to think of different ways that
     * your inputs are not appropriate and accordingly add more code.
	 */
	while((opt = getopt(argc, argv, "S:A:B:t:v")) != -1) {
		switch(opt) {
			case 'S':
				r = process_arg_S(opt, optarg);
				if (r) {
					printf("Improper S parameter\n");
					return -1;
				}
				break;
			case 'A':
				r = process_arg_A(opt, optarg);
				if (r) {
					printf("Improper A parameter\n");
					return -1;
				}
				break;
			case 'B':
				r = process_arg_B(opt, optarg);
				if (r) {
					printf("Improper B parameter\n");
					return -1;
				}
				break;
			case 't':
				trace_file = optarg;
				break;
			case 'v':
				verbose = 1;
				break;
			case '?':
			default:
				printf("Invalid options provided. \n %s\n", usage_str);
				return -1;
		}
	}

	// Open the trace file.
	trace_fp = fopen(trace_file, "r");

	if (trace_fp == NULL) {
		printf("Trace file does not exists.\n");
		return -1;
	}

	// Check if all required parameters are provided
	ret = check_parameters_valid();
	if (ret) {
		printf("One or more required parameters is missing.\n");
		printf("%s\n", usage_str);
       
		return -1;
	}

	// Initialize the system (including the cache).
	initialize();

	// Read the trace file, one line at a time.
	while (1) {
		entry = process_trace_file_line(trace_fp);
		pa = translate_address(entry);

		// Based on the access type, either read from cache or write to cache.
		if (entry.address != 0) {
			if (entry.accesstype == READ) {
				ret = read_from_cache(pa);	
			} else if (entry.accesstype == WRITE) {
				ret = write_to_cache(pa);
			} else {
				printf("This message should not be printed. Fix your code.\n");
				return -1;
			}

			// Handle verbose parameter.
			if (verbose) {
				handle_verbose(entry, ret);
			}
		} else {
			break;
		}
	}

	fclose(trace_fp);

	// Free the allocated memory.
	free_memory();

	// Print statistics at the end of the simulation.
	print_statistics();

    return 0;
} 
