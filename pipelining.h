//#include "decode.h"

struct pipeline {
    int instruct[32];
    int temp[32];
    struct decode_info* id_instruct;
    struct decode_info* ex_instruct;
    struct decode_info* me_instruct;
    struct decode_info* wb_instruct;
    int id_valid;
    int ex_valid;
    int me_valid;
    int wb_valid;
};

struct forward {
    int num_reg;
    int forwarder_is_i;
    int forwarder_is_r;
    int forwardee_is_i;
    int forwardee_is_r;
};


int is_found(struct decode_info* instruct, int reg_num);
