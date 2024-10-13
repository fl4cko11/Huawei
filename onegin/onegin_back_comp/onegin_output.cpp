#include "onegin_output.h"

void spa_output(str_ptrs *spa, size_t strs_in_buffer) {
    assert(spa != nullptr);
    assert(strs_in_buffer >= 0);

    int count = 0;
    for (int i = 0; i < strs_in_buffer; i++) {
        count = 0;
        while (spa[i].str_start + count != spa[i].str_end) {
            printf("%c", spa[i].str_start[count]);
            count++;
        }
        printf("%c", *(spa[i].str_end));
        printf("\n");
    }
}