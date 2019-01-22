#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "decode.h"
// A function that returns the specified array of bits in a number
unsigned int get_bits(unsigned int start, unsigned int end, unsigned int original){
	unsigned int k = end - start;
	return (((1 << k) -1 ) & (original >> (start -1)));

}

void decode_instruction(unsigned int instruction, struct decode_info *decode, struct register_data* registers){
	unsigned int opcode = get_bits(1,8,instruction);
	unsigned int dest_reg = get_bits(8,13,instruction);
	unsigned int funct3 = get_bits(13,16,instruction);
	struct i_type_info i_type_data;
	struct r_type_info r_type_data;

	switch(opcode){
		case 0b0000000:
			;
			unsigned int source_reg_1 = get_bits(16,21, instruction);
			unsigned int source_reg_2 = get_bits(21,26, instruction);
			unsigned int funct7 = get_bits(26,33, instruction);
			r_type_data.valid = 1;
			r_type_data.opcode = opcode;
			r_type_data.dest_reg = dest_reg;
			r_type_data.funct3 = funct3;
			r_type_data.source_reg_1 = source_reg_1;
			r_type_data.source_reg_2 = source_reg_2;
			r_type_data.funct7 = funct7;
			
			/*Read the value of the register array for the source reg vales*/
			r_type_data.source_reg_1_value = registers->registers_data[source_reg_1];
			r_type_data.source_reg_2_value = registers->registers_data[source_reg_2];
			decode->r_type = r_type_data;

			i_type_data.valid = 0;

			decode->i_type = i_type_data;

			break;
		case 0b0000001:
			;
			unsigned int source_reg = get_bits(16,21,instruction);
			unsigned int imm = get_bits(21,33,instruction);
			
			i_type_data.valid = 1;
			i_type_data.opcode = opcode;
			i_type_data.dest_reg = dest_reg;
			i_type_data.funct3 = funct3;
			i_type_data.source_reg = source_reg;
			i_type_data.imm = imm;
			/*Read the value of the source reg here*/
			i_type_data.source_reg_value = registers->registers_data[source_reg];
			decode->i_type = i_type_data;

			r_type_data.valid = 0;

			decode->r_type = r_type_data;
			break;
		default:
			printf("This instruction is not supported!\n");

	}

};
