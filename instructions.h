#ifndef _instructions_h_
#define _instructions_h_

struct instruction_memory{
	int num_instructions;
	unsigned long instructions[256];

};

void init_instruction_memory(FILE *ifp, struct instruction_memory *instruct);
void print_instructions(struct instruction_memory *instruct);
#endif
