#ifndef _data_h_
#define _data_h_

struct data_memory{
	int num_data;
	int data[256];
};

void init_data_memory(FILE *dfp, struct data_memory *mem);
void print_data(struct data_memory *mem);

#endif
