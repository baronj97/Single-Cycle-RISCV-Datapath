#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "data.h"
#include "instructionfetch.h"
#include "decode.h"

int main(int argc, char** argv){
	// Declare some file pointers
	FILE *ifp, *dfp, *rfp;
	// Open the instruction memory file
	ifp = fopen("instruction_memory.txt", "r");

	// Load the instruction memory into a structure
	struct instruction_memory instructions;
       	init_instruction_memory(ifp, &instructions);
	print_instructions(&instructions);
	// Close the file
	fclose(ifp);

	// Open the data memory file and load it into a struct
	dfp = fopen("data_memory.txt", "r");
	struct data_memory mem;
	init_data_memory(dfp, &mem);
	print_data(&mem);
	// Close the file
	fclose(dfp);

	// Initialize the program counter
	int pc;
	unsigned long int cur_instruct;
	// Iterate over each instruction while incrementing the pc
	// Decode the instructions in the loop as well
	for(pc = 0; pc < instructions.num_instructions;pc++){
		cur_instruct = instruction_fetch(&instructions, pc);
		printf("The program counter is currently: %i\n",pc);
	      	printf("The instruction is %lu\n", cur_instruct);	
	}
	
	// This is all testing... will need to move into the for-loop
	unsigned int test = 0b10000011111100000001000111011110;
	// Declare a decode struct and decode the instruction
	struct decode_info decoded_instruction;
	decode_instruction(test, &decoded_instruction);
	// Open the register file to read the registers required by the instruction
	rfp = fopen("register_file.txt", "r");
	unsigned int *val = read_register(decoded_instruction.r_type.dest_reg, rfp);

	// Some more testing. Delete this
	decoded_instruction.r_type.dest_reg = *val;
	printf("value = %u\n",*val);
	free(val);
	printf("Dest reg value: %u\n", decoded_instruction.r_type.dest_reg_value);
	// Close the register file
	fclose(rfp);
	return 0;
}
