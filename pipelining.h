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
