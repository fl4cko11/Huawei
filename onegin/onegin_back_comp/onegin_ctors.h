#ifndef ONEGIN_CTORS_H
#define ONEGIN_CTORS_H

#include "onegin_structs.h"

int strs_in_buffer(char *buffer);

buffer_params buffer_Ctor(const char* pathname);

str_ptrs *struct_ptrs_array_Ctor(buffer_params *bp);

#endif