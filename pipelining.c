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
