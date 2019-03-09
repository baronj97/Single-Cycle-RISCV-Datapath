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
        printf("Load got here\n");
        int offset = decode->i_type.i_source_reg_value + decode->i_type.i_imm;
        int reg_dest = decode->i_type.i_dest_reg; 
        reg_data->registers_data[reg_dest]= data_mem->data[offset];
        make_clean(reg_data, decode->i_type.i_dest_reg);
	}
    	else if(decode->s_type.valid && decode->s_type.opcode == 0b0100011){
        	/*STORE: the index should represent the line number you want to save it
         	*For example, sd x4, 29(x0) saves x4 to line 29 of the data_mem*/
        	int index = decode->s_type.s_imm_full + decode->s_type.s_source_reg_2;
        	int reg_dest = decode->s_type.s_source_reg_1;
        	//reg_data->registers_data[reg_dest]= data_mem->data[offset];
        	if(index > data_mem->num_data){
            		int old = data_mem->num_data;
            		data_mem->num_data = index;
            		int i;
            		for(i = old; i <= index; i++){
                		data_mem->data[i] = 0;
            		}
        	}
        	data_mem->data[index-1] = reg_data->registers_data[reg_dest];
        
    	}
    	else if(decode->i_type.valid && decode->i_type.opcode == 0b1100111){
		    reg_data->registers_data[decode->i_type.i_dest_reg] = decode->i_type.i_dest_reg_value;
    	}
    	else if(decode->uj_type.valid){
        	reg_data->registers_data[decode->uj_type.uj_dest_reg] = decode->uj_type.uj_dest_reg_value;
    	}
}
