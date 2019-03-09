#include "pipelining.h"
#include <stdlib.h>
#include "decode.h"
void init_pipeline(struct pipeline *pipe){
    pipe->id_instruct = malloc(sizeof(struct decode_info));
    pipe->ex_instruct = malloc(sizeof(struct decode_info));
    pipe->me_instruct = malloc(sizeof(struct decode_info));
    pipe->wb_instruct = malloc(sizeof(struct decode_info));
 //   pipe->id_instruct = NULL;
 //   pipe->ex_instruct = NULL;
  //  pipe->me_instruct = NULL;
  //  pipe->wb_instruct = NULL;
}

void move_pipeline(struct pipeline *pipe){
    pipe->wb_instruct = pipe->me_instruct;
    pipe->me_instruct = pipe->ex_instruct;
    pipe->ex_instruct = pipe->id_instruct;
    int i;
    for( i = 0; i < 32; i++)
        pipe->temp[i] = pipe->instruct[i];

}

/*  Return 0 if not found at all
 *  Return 1 if found in R type
 *  Return 2 if found in I typ
 */

int is_found(struct decode_info *instruct, int reg_num){
    
    if(instruct->r_type.valid){
        printf("Looking in the R type for\n");
        printf("R DEST REG: %d\n", instruct->r_type.r_dest_reg);
        if(instruct->r_type.r_dest_reg == reg_num){
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(instruct->i_type.valid){
        printf("Looking in the I type for\n");
        printf("I DEST REG: %d\n", instruct->i_type.i_dest_reg);
        if(instruct->i_type.i_dest_reg == reg_num){
            return 2;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}
