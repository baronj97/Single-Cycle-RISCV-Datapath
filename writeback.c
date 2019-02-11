#include "decode.h"
#include "data.h"
void writeback(struct decode_info* decode, struct register_data* registers){
	if(decode->r_type.valid){
		registers->registers_data[decode->r_type.r_dest_reg] = decode->r_type.r_dest_reg_value;
	}
	else if (decode->i_type.valid){
		registers->registers_data[decode->i_type.i_dest_reg] = decode->i_type.i_dest_reg_value;
	}
	else if (decode->uj_type.valid){
		registers->registers_data[decode->uj_type.uj_dest_reg] = decode->uj_type.uj_dest_reg_value;
	}
}
