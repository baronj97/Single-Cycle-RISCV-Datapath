#include <stdlib.h>
#include <stdio.h>
#define MAX_LINE_SIZE 32

// A struct to maintain all the instructions 
struct instruction_memory{
	int num_instructions;
	unsigned long instructions[256];
};

// A function that prints out all the instructions
void print_instructions(struct instruction_memory *instruct){
	printf("Number of instructions: %i\n", instruct->num_instructions);
	int i;
	for(i =0; i<instruct->num_instructions;i++)
		printf("%lu\n",instruct->instructions[i]);

}

// A function that reads the instruction file and stores them into the array
void init_instruction_memory(FILE *ifp, struct instruction_memory *instruct){
	int i = 0;
	char buffer[MAX_LINE_SIZE];
	int int_instruct;
	char *temp;
	while(fgets(buffer, MAX_LINE_SIZE, ifp)!= NULL){
		int_instruct = strtoul(buffer, &temp, 32);
		//int_instruct = atoi(buffer);
		instruct->instructions[i] = int_instruct;
		i++;
	}
	instruct->num_instructions = i;
}
