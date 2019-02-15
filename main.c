#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "data.h"
#include "instructionfetch.h"
#include "decode.h"
#include "memory.h"
#include "writeback.h"
#include "execute.h"
#include "assembler.h"
#include "config.h"

int main(int argc, char** argv){
	
    // Declare some file pointers
	FILE *ifp, *dfp;
	// Open the instruction memory file
	ifp = fopen("instruction_memory.txt", "r");

	// Load the instruction memory into a structure
	struct instruction_memory instructions;
    //init_instruction_memory(ifp, &instructions);
	assembly_to_machine(ifp, &instructions);

    instructions.num_instructions = 3;
    instructions.instructions[0] = 0b000000000000000000;
	// Close the file
	fclose(ifp);

	// Open the data memory file and load it into a struct
	dfp = fopen("data_memory.txt", "r");

	struct data_memory mem;
	
	init_data_memory(dfp, &mem);

	struct register_data registers;
	init_register_data(&registers);

	// Initialize the program counter
	int pc;
	unsigned long int cur_instruct;
	int offset = -1;
	// Iterate over each instruction while incrementing the pc
	// Decode the instructions in the loop as well
	for(pc = 0; pc < instructions.num_instructions;pc++){
		cur_instruct = instruction_fetch(&instructions, pc);
		printf("The program counter is currently: %i\n",pc);
	    printf("The instruction is %d\n", cur_instruct);	
		/*Decode the instruction*/
		struct decode_info decoded_instruction;
		decode_instruction(cur_instruct, &decoded_instruction, &registers);
		/*Execute the instruction*/
		offset = execute(&decoded_instruction);
		/*If the instruction is i type, feed it to the mem stage*/
		if(decoded_instruction.i_type.valid)
			memory(offset, &decoded_instruction, &mem);
		/*Go to writeback stage*/
		writeback(&decoded_instruction, &registers);
	}
	
	// This is all testing... will need to move into the for-loop
	// Declare a decode struct and decode the instruction
	dump_registers_to_data_memory(&registers, dfp);
	fclose(dfp);

    /*0000010 10011 01111 000 10001 0001110 */
    /* 2         19   15   0   17     14*/
    int test = 0b00000101001101111000100010001110;
    printf("opcode:%d should be 14\n", get_bits(1,8,test)); // get_bits(start+1, end+2, bit_sequence); 0 indexed
    printf("rd:%d should be 17\n", get_bits(8, 13, test));
    printf("funct3:%d should be 0\n", get_bits(13,15,test));
    printf("rs1:%d should 15\n", get_bits(16, 21, test));
    printf("rs2:%d should be 19\n", get_bits(21, 26, test));
    printf("funct7:%d should be 2\n", get_bits(26, 33, test));
	return 0;
}
