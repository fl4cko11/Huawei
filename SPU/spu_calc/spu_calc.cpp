#include "spu_calc.h"

size_t number_of_int_in_file(FILE *fp) {
    if (fp == nullptr) {
        printf("[SPU] num_of_int open error\n");
        exit(-1);
    }
    int temp = 0;
    size_t count = 0;
    while (fscanf(fp, "%d", &temp) == 1) {
        count++;
    }
    rewind(fp);
    return count;
}

void spu_code_buffer_ctor(char *codefile, my_SPU *spu, FILE *log_file) {
    if (codefile == nullptr) {
        printf ("[SPU] pathname err");
        exit(-1);
    }
    if (spu == nullptr) {
        printf ("[SPU] spu err");
        exit(-1);
    }
    if (log_file == NULL) {
        perror("[SPU] Error opening log file");
        exit(-1);
    }
    
    FILE *fp = fopen(codefile, "rb");
    if (!fp) {
        printf("[SPU] No удалось открыть файл");
        exit(-1);
    }

    // Выделяем память для буфера
    spu->size_cb = number_of_int_in_file(fp);
    fprintf(log_file, "[SPU] size code buffer: %ld\n", spu->size_cb);
    spu->code_buffer = (int *)calloc(spu->size_cb, sizeof(int));
    if (!spu->code_buffer) {
        perror("No удалось выделить память");
        fclose(fp);
        exit(-1);
    }

    //Заполняем буффер
    char cmd[50];
    int i = 0;
    while (fgets(cmd, sizeof(cmd), fp) != nullptr) {
        char *ptr_in_cmd = cmd;
        while (sscanf(ptr_in_cmd, "%d", &spu->code_buffer[i]) == 1) {
            i++;
            while (*ptr_in_cmd != '\0' && !isdigit(*ptr_in_cmd)) ptr_in_cmd++;  // пропускаем нечисловые символы
            while (isdigit(*ptr_in_cmd)) ptr_in_cmd++; // пропскаем считанное число
        }
    }
    
    fprintf(log_file, "[SPU] code buffer: ");
    for (int i = 0; i < spu->size_cb; i++) {
        fprintf(log_file, "%d ", spu->code_buffer[i]);
    }
    fprintf(log_file, "\n");

    fclose(fp);
}

void spu_ctor(my_SPU *spu, char *codefilename, char *logname) {
    if (spu ==nullptr) {
        printf("[SPU] spu error");
        exit(-1);
    }
    if (codefilename ==nullptr) {
        printf("[SPU] codefilename error");
        exit(-1);
    }
    if (logname ==nullptr) {
        printf("[SPU] logname error");
        exit(-1);
    }
    
    FILE *log_file = fopen(logname, "w");
    if (log_file == nullptr) {
        printf("[SPU] log_file open error\n");
        exit(-1);
    }
    fprintf(log_file, "[SPU] start ctor\n");
    fclose(log_file);

    my_stack stk = {};
    stack_ctor(&stk, logname);

    log_file = fopen(logname, "a");
    if (log_file == nullptr) {
        printf("[SPU] log_file open error\n");
        exit(-1);
    }

    spu->logname = logname;
    spu->stk = stk;
    spu->ip = 0;
    spu_code_buffer_ctor(codefilename, spu, log_file);
    spu->regist = (int *)calloc(num_of_registers, sizeof(int));
    if (spu->regist == nullptr) {
        printf("[SPU] regist ctor error");
    }
    fclose(log_file);
    
    spu_dump(spu);

    log_file = fopen(logname, "a");
    if (log_file == nullptr) {
        printf("[SPU] log_file open error\n");
        exit(-1);
    }
    fprintf(log_file, "[SPU] end ctor\n\n");
    fclose(log_file);
}

void spu_dump(my_SPU *spu) {
    FILE *log_file = fopen(spu->logname, "a");
    if (log_file == nullptr) {
        printf("[SPU] log_file open error\n");
        exit(-1);
    }

    fprintf(log_file, "[SPU] ip value: %d\n", spu->ip);
    fprintf(log_file, "[SPU] registers: ");
    for (int i = 0; i < num_of_registers; i++) {
        fprintf(log_file, "%d ", spu->regist[i]);
    }
    fprintf(log_file, "\n\n");
    fclose(log_file);
}

void run(char *codefilename, char *logname) {
    if (codefilename ==nullptr) {
        printf("[SPU] codefilename error");
    }
    if (logname ==nullptr) {
        printf("[SPU] logname error");
    }

    my_SPU spu = {};
    spu_ctor(&spu, codefilename, logname);

    bool if_run = true;
    stackelem_t pop1 = 0;
    stackelem_t pop2 = 0;
    stackelem_t stke = 0;

    while (if_run) {
        switch (spu.code_buffer[spu.ip]) {
            case hlt:
                stack_dtor(&spu.stk);
                if_run = false;
                break;
            case push:
                stack_push(&spu.stk, spu.code_buffer[spu.ip + 1]);
                spu.ip += 2;
                spu_dump(&spu);
                break;
            case add:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                stack_push(&spu.stk, pop1 + pop2);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case sub:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                stack_push(&spu.stk, pop2 - pop1);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case my_div:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                stack_push(&spu.stk, pop2/pop1);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case mul:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                stack_push(&spu.stk, pop2*pop1);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case my_sqrt:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_push(&spu.stk, sqrt(pop1));
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case my_sin:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_push(&spu.stk, sin(pop1));
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case my_cos:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_push(&spu.stk, cos(pop1));
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case out:
                printf("%d\n", spu.stk.data[spu.stk.size]);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case in:
                printf("enter unit: ");
                scanf("%d", &stke);
                stack_push(&spu.stk, stke);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case dump: //stk show
                stack_dump(&spu.stk);
                spu.ip += 1;
                spu_dump(&spu);
                break;
            case jump:
                spu.ip = spu.code_buffer[spu.ip + 1];
                spu_dump(&spu);
                break;
            case ja:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                if (pop2 > pop1) {
                    spu.ip = spu.code_buffer[spu.ip + 1];
                }
                else spu.ip += 2;
                spu_dump(&spu);
                break;
            case jae:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                if (pop2 >= pop1) {
                    spu.ip = spu.code_buffer[spu.ip + 1];
                }
                else spu.ip += 2;
                spu_dump(&spu);
                break;
            case jb:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                if (pop2 < pop1) {
                    spu.ip = spu.code_buffer[spu.ip + 1];
                }
                else spu.ip += 2;
                spu_dump(&spu);
                break;
            case jbe:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                if (pop2 <= pop1) {
                    spu.ip = spu.code_buffer[spu.ip + 1];
                }
                else spu.ip += 2;
                spu_dump(&spu);
                break;
            case je:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                if (pop2 == pop1) {
                    spu.ip = spu.code_buffer[spu.ip + 1];
                }
                else spu.ip += 2;
                spu_dump(&spu);
                break;
            case jne:
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                stack_pop(&spu.stk);
                pop2 = spu.stk.popped;
                if (pop2 != pop1) {
                    spu.ip = spu.code_buffer[spu.ip + 1];
                }
                else spu.ip += 2;
                spu_dump(&spu);
                break;
            case pushr: // из регистра в стк
                stke = spu.regist[spu.code_buffer[spu.ip + 1]];
                stack_push(&spu.stk, stke);
                spu.ip += 2;
                spu_dump(&spu);
                break;
            case popr: // из стека в регистр
                stack_pop(&spu.stk);
                pop1 = spu.stk.popped;
                spu.regist[spu.code_buffer[spu.ip + 1]] = pop1;
                spu.ip += 2;
                spu_dump(&spu);
                break;
        }
    }
}