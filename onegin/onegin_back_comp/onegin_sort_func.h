#ifndef ONEGIN_SORT_FUNC_H
#define ONEGIN_SORT_FUNC_H

#include "onegin_structs.h"

int len_s(str_ptrs s1);

int max_string(str_ptrs s1, str_ptrs s2);

int str_compare(str_ptrs s1, str_ptrs s2);

void string_swap(str_ptrs *spa, int index_1, int index_2, size_t strs_in_buffer);

void sort_func(str_ptrs *spa, int len_list);

#endif