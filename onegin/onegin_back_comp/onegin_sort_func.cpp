#include "onegin_sort_func.h"

int len_s(str_ptrs s1) {
    assert(s1.str_start != nullptr && s1.str_start != nullptr);
    return s1.str_end - s1.str_start;
}

int max_string(str_ptrs s1, str_ptrs s2) {
    assert(s1.str_start != nullptr && s1.str_start != nullptr && s2.str_start != nullptr && s2.str_start != nullptr);

    int lens1 = len_s(s1);
    int lens2 = len_s(s2);
    return lens1 > lens2 ? lens1 : lens2;  
}

int str_compare(str_ptrs s1, str_ptrs s2) {
    assert(s1.str_start != nullptr && s1.str_start != nullptr && s2.str_start != nullptr && s2.str_start != nullptr);

    int len_longest_str = max_string(s1, s2);
    int symbol_s1 = 0;
    int symbol_s2 = 0;
    for (int i = 0; i < len_longest_str; i++) {
        if ((s1.str_end - symbol_s1) != s1.str_start && (s2.str_end - symbol_s2) != s2.str_start) {
            while (!isalpha(s1.str_end[-symbol_s1]) && (s1.str_end - symbol_s1) != s1.str_start) {
                symbol_s1++;
            }
            while (!isalpha(s2.str_end[-symbol_s2]) && (s2.str_end - symbol_s2) != s2.str_start) {
                symbol_s2++;
            }
            if (tolower(s1.str_end[-symbol_s1]) != tolower(s2.str_end[-symbol_s2])) {
                return tolower(s1.str_end[-symbol_s1]) - tolower(s2.str_end[-symbol_s2]);
            }
            symbol_s1++;
            symbol_s2++;
        }
        else return 0;
    }
    return 0;
}

void string_swap(str_ptrs *spa, int index_1, int index_2, size_t strs_in_buffer) {
    assert(spa != nullptr);
    assert(index_1 >=0 && index_2 >= 0);
    assert(strs_in_buffer >= 0);

    str_ptrs temp = spa[index_1];
    spa[index_1] = spa[index_2];
    spa[index_2] = temp;
}

void sort_func(str_ptrs *spa, int len_list) {
    assert(spa != nullptr);
    assert(len_list >= 0);

    int it = 0;
    bool is_changes = true;
    int diff_s = 0;
    while (is_changes){
        is_changes = false;
        for (int j = 0; j <= (len_list - it - 2); j++){
            diff_s = str_compare(spa[j], spa[j+1]);
            if(diff_s > 0){
                string_swap(spa, j, j+1, len_list);
                is_changes = true;
            }
        }
        it++;
    }
}