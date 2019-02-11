#include <stdio.h>
#include <stdlib.h>
#include "decode.h"
/*
 *
 * The execute stage
 * Check if it is an R type or I type
 * 	R Type
 * 		Decipher the actual instruction
 * 		Switch statement for the types I want to support
 * 		Add, Sub for starters
 * 		
 * 	I Type
 *		Decipher the instruction
 *		Switch statement for the type of instructions
 *		Return the calculated address
 * 			
 */	

int execute(struct decode_info *decode){
	/* If it is an r type instruction, do this */
	if(decode->r_type.valid){
		switch(decode->r_type.opcode){
			case 0b000:
				/*ADD*/
				decode->r_type.dest_reg_value = decode->r_type.source_reg_1_value + decode->r_type.source_reg_2_value;
				break;
			case 0b001:
				/*For sub*/
				decode->r_type.dest_reg_value = decode->r_type.source_reg_1_value - decode->r_type.source_reg_2_value;
				break;
			case 0b100:
				/*XOR*/
				decode->r_type.dest_reg_value = decode->r_type.source_reg_1_value ^ decode->r_type.source_reg_2_value;
				break;
			case 0b101:
				/*OR*/
				decode->r_type.dest_reg_value = decode->r_type.source_reg_1_value | decode->r_type.source_reg_2_value;
				break;
			case 0b111:
				/*AND*/
				decode->r_type.dest_reg_value = decode->r_type.source_reg_1_value & decode->r_type.source_reg_2_value;
				break;
			default:
				printf("This type of operation is not supported\n");

		
		}
		/* The return value for this function is the address for the MEM stage
		 * Since R type does not require this, let's return -1 */
		return -1;
		
	
	}
	/* It must be an I type*/
	else{
		unsigned int offset;
		switch(decode->i_type.opcode){
			case 0b001:
				/*Load*/
				offset = decode->i_type.dest_reg;
				break;
			case 0b010:
				/*Store*/
				offset = decode->i_type.dest_reg;
				break;
			default:
				printf("This type of operation is not supported\n");

		}

		return offset;
	}
	/*If we get here, something went really wrong*/
	return -1;


}
