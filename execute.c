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

int execute(struct decode_info *decode, int pc){
	/* R-type */
	if(decode->r_type.valid){
		switch(decode->r_type.r_funct3){
			case 0b000:
				if ((decode->r_type.r_funct7) == 0b0000000){
					/*ADD*/
					decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value + decode->r_type.r_source_reg_2_value;
					break;
				} else {
					/*SUB*/
					decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value - decode->r_type.r_source_reg_2_value;
					break;
				}
				break;
			case 0b001:
				/*SLL* Shift Left logical*/
				decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value << decode->r_type.r_source_reg_2_value;
				printf("Shifting: %d << %d\n", decode->r_type.r_source_reg_1_value, decode->r_type.r_source_reg_2_value  );
                break;
			case 0b101:
				/*SRL* Shift Right Logical*/
				decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value >> decode->r_type.r_source_reg_2_value;
				break;
			case 0b100:
				/*XOR*/
				decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value ^ decode->r_type.r_source_reg_2_value;
				break;
			case 0b110:
				/*OR*/
				decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value | decode->r_type.r_source_reg_2_value;
				break;
			case 0b111:
				/*AND*/
				decode->r_type.r_dest_reg_value = decode->r_type.r_source_reg_1_value & decode->r_type.r_source_reg_2_value;
				break;
			default:

				printf("This type of operation is not supported\n");
		}
		/* The return value for this function is the address for the MEM stage
		 * Since R type does not require this, let's return -1 */
		return -1;
	} /* I-type */
	else if (decode->i_type.valid){
		unsigned int offset;
		switch(decode->i_type.i_funct3){
			case 0b000:
				if ((decode->i_type.opcode) == 0b0010011){
					/*ADDI*/
					decode->i_type.i_dest_reg_value = decode->i_type.i_source_reg_value + decode->i_type.i_imm;
					break;
				} else {
					/*JALR*/
				        // NOT DONE!	
                    decode->i_type.i_dest_reg_value = pc;               
                    printf("[JALR] PC Before JALR: %d\n", pc);
                    printf("[JALR] Source Reg Value: %d\n", decode->i_type.i_source_reg_value);
                    printf("[JALR] Imm Value: %d\n", decode->i_type.i_imm);
                    pc  = (decode->i_type.i_source_reg_value + decode->i_type.i_imm) - 1;
                    printf("[JALR] PC After JALR: %d\n", pc);

                    return pc;   
					break;		
				}
				break;
			case 0b001:
				/*SLLI*/
				decode->i_type.i_dest_reg_value = decode->i_type.i_source_reg_value << decode->i_type.i_imm;
				printf("slli\n");
                break;
			case 0b100:
				/*XORI*/
				decode->i_type.i_dest_reg_value = decode->i_type.i_source_reg_value ^ decode->i_type.i_imm;
				break;
			case 0b101:
				/*SLRI*/
				decode->i_type.i_dest_reg_value = decode->i_type.i_source_reg_value >> decode->i_type.i_imm;
				break;
			case 0b110:
				/*ORI*/
				decode->i_type.i_dest_reg_value = decode->i_type.i_source_reg_value | decode->i_type.i_imm;
				break;
			case 0b111:
				/*ANDI*/
				decode->i_type.i_dest_reg_value = decode->i_type.i_source_reg_value & decode->i_type.i_imm;
				break;
			case 0b011:
				/*Load*/
				offset = decode->i_type.i_dest_reg;
				break;
			default:
				printf("This type of operation is not supported");
		}
return -1;
	} /* S-type */
	else if (decode->s_type.valid){
		switch(decode->s_type.s_funct3){
			case 0b011:
				/*SD*/
			    decode->s_type.s_imm_full = (decode->s_type.s_imm_1 << 4) | decode->s_type.s_imm_2;
                break;
			default:
				printf("This type of operation is not supported");
		}	
        return -1;
	} /* SB-type */ 
	else if (decode->sb_type.valid){
		switch(decode->sb_type.sb_funct3){
			case 0b000:
				/*BEQ*/
				if (decode->sb_type.sb_source_reg_1_value == decode->sb_type.sb_source_reg_2_value) {
				    	int result = (decode->sb_type.sb_imm_1 << 4) | decode->sb_type.sb_imm_2;
                    			pc = (decode->sb_type.sb_imm_1 << 4) | (decode->sb_type.sb_imm_2) - 1;
				}
                		else {
                    			pc = -1;
                		}
			   	break;
			case 0b001:
				/*BNE*/
				if (decode->sb_type.sb_source_reg_1_value != decode->sb_type.sb_source_reg_2_value) {
                        		pc = (decode->sb_type.sb_imm_1 << 4) | (decode->sb_type.sb_imm_2) - 1;
				}
                		else{
                    			pc = -1;
               		 	}
				break;
			case 0b100:
				/*BLT*/
				if (decode->sb_type.sb_source_reg_1_value < decode->sb_type.sb_source_reg_2_value) {
                    			pc = (decode->sb_type.sb_imm_1 << 4) | (decode->sb_type.sb_imm_2) - 1;
				}
                		else{
                    			pc = -1;
                		}
				break;
			case 0b101:
				/*BGE*/
				if (decode->sb_type.sb_source_reg_1_value >= decode->sb_type.sb_source_reg_2_value) {
                    			pc = (decode->sb_type.sb_imm_1 << 4) | (decode->sb_type.sb_imm_2) - 1;
				}
                		else{
                    			pc = -1;
                		}
				break;
			default: 
				printf("This type of operation is not supported");
		} 
        return pc;
	} /* UJ-type */
	else if (decode->uj_type.valid){
		switch(decode->uj_type.opcode){
			case 0b0101111:
				/*JAL*/
				/*decode->uj_type.uj_dest_reg_value = PC + 4; PC - PC + (imm,1b'0)*/
                		//decode->uj_type.uj_dest_reg_value = pc;               
				printf("[UJ TYPE] PC Before JAL: %d\n", pc);
                decode->uj_type.uj_dest_reg_value = pc;
				pc = decode->uj_type.uj_imm - 1;

				printf("[UJ TYPE] PC: %d\n", pc);
				printf("Dest value of UJ: %d\n", decode->uj_type.uj_dest_reg_value);
				break;
			default:
				printf("This type of operation is not supported in exec\n");
		}
        return pc;
	} 
}
