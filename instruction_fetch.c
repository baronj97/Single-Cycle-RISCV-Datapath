#include <stdlib.h>
#include <stdio.h>
#include "instructions.h"

// A fnction to return the instruction based on the program counter
unsigned long int instruction_fetch(struct instruction_memory *instruct, int pc){
	return  instruct->instructions[pc];

}
