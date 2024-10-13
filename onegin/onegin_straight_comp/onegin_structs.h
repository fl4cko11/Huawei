#ifndef ONEGIN_STRUCTS_H
#define ONEGIN_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *str_start;
    char *str_end;
} str_ptrs;

typedef struct {
    char *buffer;
    size_t strs_in_buffer;
} buffer_params;

#endif