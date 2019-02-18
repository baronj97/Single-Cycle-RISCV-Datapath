#include <stdlib.h>
#include "data.h"
#include "decode.h"
/* This is the memory state
 * If the instruction is I type, we need to go through here
 *
 * Check if the instruction is I type
 * If so,
 * 	Read the index of the data memory array based on the calculation from the execue stage
 */


void memory(struct decode_info* decode, struct data_memory* data_mem, struct register_data* reg_data){
	/*Let's just make sure this is the right type*/
	if(decode->i_type.valid && decode->i_type.opcode == 0b0000011){
        int offset = decode->i_type.i_source_reg_value + decode->i_type.i_imm;
        
        int reg_dest = decode->i_type.i_dest_reg;
        printf("reg_dest_value = %d\n", decode->i_type.i_source_reg_value);
        printf("imm = %d\n", decode->i_type.i_imm);
        printf("Reading from data_mem:%d\n", data_mem->data[offset]);
        
        reg_data->registers_data[reg_dest]= data_mem->data[offset];
	}
    else if(decode->s_type.valid && decode->s_type.opcode == 0b0100011){
        int index = decode->s_type.s_imm_full + decode->s_type.s_source_reg_2_value;
        int reg_dest = decode->s_type.s_source_reg_1;
        printf("S type is here\n");
		//reg_data->registers_data[reg_dest]= data_mem->data[offset];
        data_mem->data[index] = reg_data->registers_data[reg_dest];
    }
    else if(decode->i_type.valid && decode->i_type.opcode == 0b1100111){
        reg_data->registers_data[decode->i_type.i_dest_reg] = decode->i_type.i_dest_reg_value;
    }
    else if(decode->uj_type.valid){
        reg_data->registers_data[decode->uj_type.uj_dest_reg] = decode->uj_type.uj_dest_reg_value;
    }
}
