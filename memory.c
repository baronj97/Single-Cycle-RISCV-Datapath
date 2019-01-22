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


void memory(unsigned int offset, struct decode_info* decode, struct data_memory* data_mem){
	/*Let's just make sure this is the right type*/
	if(decode->i_type.valid){
		decode->i_type.dest_reg_value = data_mem->data[offset];
	}
}
