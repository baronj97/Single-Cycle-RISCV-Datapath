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
	// All opcode in same location!i
	unsigned int opcode = get_bits(1,7,instruction);
	
	struct r_type_info r_type_data;
	struct i_type_info i_type_data;
	struct s_type_info s_type_data;
	struct sb_type_info sb_type_data;
	struct uj_type_info uj_type_data;

	switch(opcode){
		// R-type (add sub sll srl xor or and)
		case 0b0110011:
			;
			unsigned int r_dest_reg = get_bits(8,12,instruction);
			unsigned int r_funct3 = get_bits(13,15,instruction);
			unsigned int r_source_reg_1 = get_bits(16,20,instruction);
			unsigned int r_source_reg_2 = get_bits(21,25,instruction);
			unsigned int r_funct7 = get_bits(26,32,instruction);

			r_type_data.valid = 1;
			r_type_data.opcode = opcode;
			r_type_data.r_dest_reg = r_dest_reg;
			r_type_data.r_funct3 = r_funct3;
			r_type_data.r_source_reg_1 = r_source_reg_1;
			r_type_data.r_source_reg_2 = r_source_reg_2;
			r_type_data.r_funct7 = r_funct7;
			
			/*Read the value of the register array for the source reg vales*/
			r_type_data.r_source_reg_1_value = registers->registers_data[r_source_reg_1];
			r_type_data.r_source_reg_2_value = registers->registers_data[r_source_reg_2];
			decode->r_type = r_type_data;

			i_type_data.valid = 0;
			s_type_data.valid = 0;
			sb_type_data.valid = 0;
			uj_type_data.valid = 0;

			decode->i_type = i_type_data;
			decode->s_type = s_type_data;
			decode->sb_type = sb_type_data;
			decode->uj_type = uj_type_data;

			break;
		// I-type (addi slli xori srli ori andi)
		//case 0b0010011:
		//	;
		// Special I-type (ld) 
		//case 0b0000011:
		//	;
		// Special I-type (jalr)
		case 0b1100111:
			;
			unsigned int i_dest_reg = get_bits(8,12,instruction);
			unsigned int i_funct3 = get_bits(13,15,instruction);
			unsigned int i_source_reg = get_bits(16,20,instruction);
			unsigned int i_imm = get_bits(21,32,instruction);
			
			i_type_data.valid = 1;
			i_type_data.opcode = opcode;
			i_type_data.i_dest_reg = i_dest_reg;
			i_type_data.i_funct3 = i_funct3;
			i_type_data.i_source_reg = i_source_reg;
			i_type_data.i_imm = i_imm;

			/*Read the value of the source reg here*/
			i_type_data.i_source_reg_value = registers->registers_data[i_source_reg];
			decode->i_type = i_type_data;

			r_type_data.valid = 0;
			s_type_data.valid = 0;
			sb_type_data.valid = 0;
			uj_type_data.valid = 0;

			decode->r_type = r_type_data;
			decode->s_type = s_type_data;
			decode->sb_type = sb_type_data;
			decode->uj_type = uj_type_data;
			
			break;
		// S-type (sd)
		case 0b0100011:
			;
			unsigned int s_imm_1 = get_bits(8,12,instruction);
			unsigned int s_funct3 = get_bits(13,15,instruction);
			unsigned int s_source_reg_1 = get_bits(16,20,instruction);
			unsigned int s_source_reg_2 = get_bits(21,25,instruction);
			unsigned int s_imm_2 = get_bits(26,32,instruction);
			
			s_type_data.valid = 1;
			s_type_data.opcode = opcode;
			s_type_data.s_imm_1 = s_imm_1;
			s_type_data.s_funct3 = s_funct3;
			s_type_data.s_source_reg_1 = s_source_reg_1;
			s_type_data.s_source_reg_2 = s_source_reg_2;
			s_type_data.s_imm_2 = s_imm_2;

			/*Read the value of the source reg here*/
			s_type_data.s_source_reg_1_value = registers->registers_data[s_source_reg_1];
			s_type_data.s_source_reg_2_value = registers->registers_data[s_source_reg_2];
			decode->s_type = s_type_data;

			r_type_data.valid = 0;
			i_type_data.valid = 0;
			sb_type_data.valid = 0;
			uj_type_data.valid = 0;

			decode->r_type = r_type_data;
			decode->i_type = i_type_data;
			decode->sb_type = sb_type_data;
			decode->uj_type = uj_type_data;
			
			break;
		// SB-type (beq bne blt bge)
		case 0b1100011:
			;
			unsigned int sb_imm_1 = get_bits(8,12,instruction);
			unsigned int sb_funct3 = get_bits(13,15,instruction);
			unsigned int sb_source_reg_1 = get_bits(16,20,instruction);
			unsigned int sb_source_reg_2 = get_bits(21,25,instruction);
			unsigned int sb_imm_2 = get_bits(26,32,instruction);
			
			sb_type_data.valid = 1;
			sb_type_data.opcode = opcode;
			sb_type_data.sb_imm_1 = sb_imm_1;
			sb_type_data.sb_funct3 = sb_funct3;
			sb_type_data.sb_source_reg_1 = sb_source_reg_1;
			sb_type_data.sb_source_reg_2 = sb_source_reg_2;
			sb_type_data.sb_imm_2 = sb_imm_2;

			/*Read the value of the source reg here*/
			sb_type_data.sb_source_reg_1_value = registers->registers_data[sb_source_reg_1];
			sb_type_data.sb_source_reg_2_value = registers->registers_data[sb_source_reg_2];
			decode->sb_type = sb_type_data;

			r_type_data.valid = 0;
			i_type_data.valid = 0;
			s_type_data.valid = 0;
			uj_type_data.valid = 0;

			decode->r_type = r_type_data;
			decode->i_type = i_type_data;
			decode->s_type = s_type_data;
			decode->uj_type = uj_type_data;
			
			break;
		// UJ-type (jal)
		case 0b1101111:
			;
			unsigned int uj_dest_reg = get_bits(8,12,instruction);
			unsigned int uj_imm = get_bits(13,32,instruction);
			
			uj_type_data.valid = 1;
			uj_type_data.opcode = opcode;
			uj_type_data.uj_dest_reg = uj_dest_reg;
			uj_type_data.uj_imm = uj_imm;

			decode->uj_type = uj_type_data;

			r_type_data.valid = 0;
			i_type_data.valid = 0;
			s_type_data.valid = 0;
			sb_type_data.valid = 0;

			decode->r_type = r_type_data;
			decode->i_type = i_type_data;
			decode->s_type = s_type_data;
			decode->sb_type = sb_type_data;
			
			break;
		// No-type	
		default:
			printf("This instruction is not supported!\n");

	}

};
