#include "onegin_structs.h"
#include "onegin_ctors.h"
#include "onegin_sort_func.h"
#include "onegin_output.h"

int main() {
    const char *pathname = "onegin.txt";
    buffer_params bp = buffer_Ctor(pathname);
    str_ptrs *spa = struct_ptrs_array_Ctor(&bp);
    sort_func(spa, bp.strs_in_buffer);
    spa_output(spa, bp.strs_in_buffer);
    return 0;
}