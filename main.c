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

void print_instruction(int* instruct){
    int i;
    for(i = 0; i < 32; i++)
        printf("%d", instruct[i]);
    printf("\n");

}


int main(int argc, char** argv){
	
    // Declare some file pointers
	FILE *ifp, *dfp;
	// Open the instruction memory file
	ifp = fopen("instruction_memory.txt", "r");

	// Load the instruction memory into a structure
	struct instruction_memory instructions;
    	//init_instruction_memory(ifp, &instructions);
	assembly_to_machine(ifp, &instructions);


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
	int branch = -1;
	// Iterate over each instruction while incrementing the pc
	// Decode the instructions in the loop as well
	for(pc = 0; pc < instructions.num_instructions;pc++){
		printf("The PC is: %d\n", pc);
        int instruct[32];
		instruction_fetch(&instructions, pc, instruct);
 //       print_instruction(instruct);
        /*Decode the instruction*/
		struct decode_info decoded_instruction;
        decode_instruction(instruct, &decoded_instruction, &registers);
		/*Execute the instruction*/
		branch = execute(&decoded_instruction, pc);
		/*If the instruction is i type, feed it to the mem stage*/
		if(decoded_instruction.i_type.valid)
			memory(&decoded_instruction, &mem, &registers);
		/*Go to writeback stage*/
		writeback(&decoded_instruction, &registers);
        
	// Debug
		
	if(branch > 0){
            pc = branch - 1;
        }  
        //printf("%d\n", decoded_instruction.i_type.i_i);
        print_data(&mem);
        print_registers(&registers);

	}
	
	// This is all testing... will need to move into the for-loop
	// Declare a decode struct and decode the instruction
	dump_registers_to_data_memory(&registers, dfp);
	fclose(dfp);

	return 0;
}
