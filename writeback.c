#include "decode.h"
#include "data.h"
void writeback(struct decode_info* decode, struct register_data* registers){
	if(decode->r_type.valid){
		registers->registers_data[decode->r_type.dest_reg] = decode->r_type.dest_reg_value;
	}
	else{
		registers->registers_data[decode->i_type.dest_reg] = decode->i_type.dest_reg_value;
	}
	

}
