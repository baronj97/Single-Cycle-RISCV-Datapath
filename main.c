#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "instructions.h"
#include "data.h"
#include "instructionfetch.h"
#include "decode.h"
#include "memory.h"
#include "writeback.h"
#include "execute.h"
#include "assembler.h"
#include "config.h"
#include "pipelining.h"


void print_instruction(int* instruct){
    int i;
    for(i = 0; i < 32; i++)
    printf("%d", instruct[i]);
    printf("\n");
}


int main(int argc, char** argv){	
    // Declare some file pointers
    FILE *ifp, *dfp, *cfp;

    if(argc < 3){
        printf("USAGE: ./simulator <config.txt> <instruction_memory_1.txt> ...\n");
        return 1;
    }

    cfp = fopen(argv[1], "r");
    char buffer[100];
    char *token;
    int num_cores;
    float freq;

    fgets(buffer, 100, cfp);
    token = strtok(buffer, ":");
    token = strtok(NULL, ":");
    token++;
    num_cores = atoi(token);
    fgets(buffer, 100, cfp);
    token = strtok(buffer, ":");
    token = strtok(NULL, ":");
    token++;
    freq = atof(token);
    if(argc != num_cores + 2){
        printf("ERROR: You did not supply enough instruction memory files\nHint: Provide 1 file for each core\nYour configuration file specifies %d cores, so please run again with %d instruction memory files\n", num_cores, num_cores);
        return 1;
    }
    
    struct core cores[num_cores];
    int core_index;
    for(core_index = 0; core_index < num_cores; core_index++){
        cores[core_index].core_id = core_index;
        cores[core_index].freq = freq;
        cores[core_index].status = 1;

        ifp = fopen(argv[2 + core_index], "r");
        struct instruction_memory instructions;
        assembly_to_machine(ifp, &instructions);
        fclose(ifp);
        cores[core_index].instruction_set = instructions;
    }
    
    // Open the data memory file and load it into a struct
    dfp = fopen("data_memory.txt", "r+");

    struct data_memory mem;
    init_data_memory(dfp, &mem);

    struct instruction_memory core_instructs;

    struct register_data registers;
    init_register_data(&registers);
    double time_taken = 0;
    int i = 0;
    for(i = 0; i < num_cores; i++){
        clock_t t;
        t = clock();
        core_instructs.num_instructions = cores[i].instruction_set.num_instructions;
	printf("NUM_INSTRUCTS : %d\n", core_instructs.num_instructions);
        int j;
        for(j =0; j < 256; j++){
            int k;
            for(k =0; k < 32; k++){
                core_instructs.instructions_array[j][k] = cores[i].instruction_set.instructions_array[j][k];
            }
        }
        
	// Initialize the program counter
	int pc;
	int branch = -1;
	int flush = 0;
    int cur_flush = 0;
    int stall = 0;
    int stall_count = 0;
	// Iterate over each instruction while incrementing the pc
	// Decode the instructions in the loop as well
	struct pipeline *pipe = malloc(sizeof(struct pipeline));
    init_pipeline(pipe);
    int setup = 1;
    for(pc = 0; pc < core_instructs.num_instructions + 5;pc++){
        printf("PC %d\n", pc);
        if(setup > 4 && pc < core_instructs.num_instructions + 4 && pipe->wb_instruct != NULL && flush != 1){
            if (stall){
                stall_count--;
            }
            else{
                printf("writeback with id %d\n", pipe->wb_instruct->id);
                writeback(pipe->wb_instruct, &registers);
            // print_registers(&registers);
           }
        }   
        if(setup > 3 && pc < core_instructs.num_instructions + 3 && pipe->me_instruct != NULL && flush != 1){
            if (stall) {
                stall_count--;
            }
            else{
                printf("Memory with id %d\n", pipe->me_instruct->id);
                if(pipe->me_instruct->i_type.valid || pipe->me_instruct->s_type.valid || pipe->me_instruct->uj_type.valid){
	                memory(pipe->me_instruct, &mem, &registers);
                }
                // print_registers(&registers);
            }
        }

        if(setup > 2 && pc < core_instructs.num_instructions + 2 && pipe->ex_instruct != NULL && flush != 1){
            if (stall) {
                stall_count--;
            }
            else{
                printf("Executed! with id %d\n", pipe->ex_instruct->id);
                branch = execute(pipe->ex_instruct, pc);
                if(pc == 2){
                    flush = 1;
                    printf("Flush triggered\n");
                 }
                // print_registers(&registers);
           }
        }
        if(setup > 1 && pc < core_instructs.num_instructions + 1){
            if(stall && !stall_count){
                stall = 0;
            }else{
            printf("Decoded!\n");
         //   print_instruction(pipe->temp);
            decode_instruction(pipe->temp, pipe->id_instruct, &registers);
           // print_registers(&registers);
            pipe->id_instruct->id = pc;
            if(pc == 2){
                printf("Stalling!\n");
                stall = 1;
                stall_count = 3;
                core_instructs.num_instructions = core_instructs.num_instructions + 3;
            }
            }
        }
        if(pc < core_instructs.num_instructions){    
            if(!stall){
            
            printf("Fetched\n");
    
            instruction_fetch(&core_instructs, pc, pipe->instruct);
             //   print_registers(&registers);
            }
        }
        if(branch > 0){
            pc = branch - 1;
          	branch = -1;
       }
        struct decode_info *temp_id = malloc(sizeof(struct decode_info));
        temp_id = pipe->id_instruct;
        struct decode_info *temp_me = malloc(sizeof(struct decode_info));
        temp_me = pipe->me_instruct;
        struct decode_info *temp_ex = malloc(sizeof(struct decode_info));
        temp_ex = pipe->ex_instruct;

        if (flush || stall){
            /*pipe->wb_instruct = NULL;
            pipe->me_instruct = NULL;
            pipe->ex_instruct = NULL;
            pipe->id_instruct = NULL; */
        }
        else{
            pipe->wb_instruct->r_type = temp_me->r_type;
            pipe->me_instruct->r_type = temp_ex->r_type;
            pipe->ex_instruct->r_type = temp_id->r_type;
            pipe->wb_instruct->i_type = temp_me->i_type;
            pipe->me_instruct->i_type = temp_ex->i_type;
            pipe->ex_instruct->i_type = temp_id->i_type;

        
            pipe->wb_instruct->s_type = temp_me->s_type;
            pipe->me_instruct->s_type = temp_ex->s_type;
            pipe->ex_instruct->s_type = temp_id->s_type;

            pipe->wb_instruct->sb_type = temp_me->sb_type;
            pipe->me_instruct->sb_type = temp_ex->sb_type;
            pipe->ex_instruct->sb_type = temp_id->sb_type;

            pipe->wb_instruct->uj_type = temp_me->uj_type;
            pipe->me_instruct->uj_type = temp_ex->uj_type;
            pipe->ex_instruct->uj_type = temp_id->uj_type;
            if(pc < core_instructs.num_instructions){
                int l;
                for(l =0; l < 32; l++)
                    pipe->temp[l] = pipe->instruct[l];
            }
        }
        if(flush){
            flush = 0;
            setup = 0;
        }
        if(!stall)
        setup++;
        //print_registers(&registers);
        printf("Next cycle\n");
	}

        t = clock() - t;
        time_taken = (time_taken + ((double) t)) * (1 / freq);
    }

    int a;
    size_t ret;
    fseek(dfp, 0, SEEK_SET);
    for(a =0; a <mem.num_data ; a++){
        fprintf(dfp, "%d\n", mem.data[a]);
    }
    
    // Print register values
    print_registers(&registers);
    printf("Time elapsed is: %.3fns\n", (time_taken / CLOCKS_PER_SEC) * 1000000000);
    
    fclose(dfp);  
    return 0;
}
