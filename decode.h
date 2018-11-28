struct r_type_info{
	int valid;
	unsigned int opcode;
	unsigned int dest_reg;
	unsigned int funct3;
	unsigned int source_reg_1;
	unsigned int source_reg_2;
	unsigned int funct7;

	unsigned int dest_reg_value;
	unsigned int source_reg_1_value;
	unsigned int source_reg_2_value;

};

struct i_type_info{
	int valid;
	unsigned int opcode;
	unsigned int dest_reg;
	unsigned int funct3;
	unsigned int source_reg;
	unsigned int imm;
	
	unsigned int dest_reg_value;
	unsigned int source_reg_value;
};


struct decode_info{
	struct r_type_info r_type;
	struct i_type_info i_type;	

};


unsigned int get_bits(unsigned int start, unsigned int end, unsigned int original);

unsigned int* read_register(unsigned int reg_address,FILE *rfp);

void decode_instruction(unsigned int instruction, struct decode_info *decode);
