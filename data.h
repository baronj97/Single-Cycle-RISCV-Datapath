#ifndef _data_h_
#define _data_h_
#include <stdio.h>
struct data_memory{
	int num_data;
	int data[256];
};
struct register_data{
	int num_registers;
	int registers_data[32];
    int registers_valid[32];
};
void init_data_memory(FILE *dfp, struct data_memory *mem);
void print_data(struct data_memory *mem);
void init_register_data(struct register_data *registers);
void make_dirty(struct register_data *registers, int register_index);
void make_clean(struct register_data *registers, int register_index);
#endif
