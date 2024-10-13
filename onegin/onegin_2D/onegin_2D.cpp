#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

const int max_words = 100;

struct onegin_params{
    char** string_list;
    int* value_list;
    int len_lists;
};

int max_string (char *s1, char *s2){
    int lens1 = strlen(s1);
    int lens2 = strlen(s2);
    return lens1 > lens2 ? lens1 : lens2;
    
}

int find_index(int *value_list, int value_for_s, int len_list){
    for (int i = 0; i < len_list; ++i) {
        if (value_list[i] == value_for_s) {
            return i;
        }
    }
    return -1;
}

int string_compare(char *s1, char *s2){
    int i = 0;
    int j = 0;

    while (s1[i] != '\0' && !isalpha(s1[i])) {
        i++;
    }

    while (s2[j] != '\0' && !isalpha(s2[j])) {
        j++;
    }

    for (int k = i, t = j; (k < max_string(s1, s2)) && (t < max_string(s1, s2)); k++, t++){
        if (s1[k] != s2[t]){
            return s1[k] - s2[t];
        }
        if ((s1[k] == '\0') || (s2[t] == '\0')){
            return 0;
        }
    }
    return 0;
}

int *string_replacer(int *value_list, int value_for_s1, int value_for_s2, int diff_s, int len_list){
    int index_s1 = find_index(value_list, value_for_s1, len_list);
    int index_s2 = find_index(value_list, value_for_s2, len_list);
    if (diff_s > 0){
        int temp = value_list[index_s1];
        value_list[index_s1] = value_list[index_s2];
        value_list[index_s2] = temp;
        return value_list;
    }
    else return value_list;
}

int *sort_func(onegin_params *onp){ /*bubble*/
    int i = 0;
    int flag = 1;
    int diff_s = 0;
    while (flag == 1){
        flag = 0;
        for (int j = 0; j <= (onp->len_lists - i - 2); j++){
            diff_s = string_compare(onp->string_list[onp->value_list[j]], onp->string_list[onp->value_list[j+1]]);
            if(diff_s > 0){
                onp->value_list = string_replacer(onp->value_list, onp->value_list[j], onp->value_list[j+1], diff_s, onp->len_lists);
                flag = 1;
            }
        }
        i++;
    }
    return onp->value_list;
}

void onegin_output(onegin_params *onp){
    for (int i = 0; i < onp->len_lists; i++){
        printf("%s\n", onp->string_list[onp->value_list[i]]);
    }
}

int strings_in_txt(FILE *txt_file) {
    char str[max_words];
    int count = 0;
    // Считываем строки из файла
    while (fscanf(txt_file, "%[^\n]\n", str) == 1) {
        count++;
    }
    // Возвращаем указатель в начало файла для повторного чтения
    rewind(txt_file);
    return count;
}

onegin_params onegin_input(FILE *oneginfp){
    onegin_params onp = {NULL, NULL};

    //кол-во строк для 2d массива
    int strings_onegin = strings_in_txt(oneginfp);

    // Выделяем память для строк
    char **str_list = (char **)malloc(strings_onegin * sizeof(char *));
    if (str_list == NULL) {
        perror("Failed to allocate memory for string list");
        fclose(oneginfp);
        assert(str_list != NULL);
    }

    // Выделяем память для столбцов
    for (int i = 0; i < strings_onegin; i++) {
        str_list[i] = (char *)malloc(max_words * sizeof(char));
        if (str_list[i] == NULL) {
            perror("Failed to allocate memory for a word");
            // Освобождаем уже выделенную память в случае ошибки
            for (int j = 0; j < i; j++) {
                free(str_list[j]);
            }
            free(str_list);
            fclose(oneginfp);
            assert(str_list[i] != NULL);
        }
    }

    // Считываем слова в массив строк
    for (int i = 0; i < strings_onegin; i++) {
        if (fgets(str_list[i], max_words, oneginfp) == NULL) {
            break;  // Выход из цикла, если достигнут конец файла
        }
        // Убираем символ новой строки, если он есть
        str_list[i][strcspn(str_list[i], "\n")] = '\0';
    }

    int *value_list = (int *)malloc(strings_onegin * sizeof(int));
    for (int i = 0; i < strings_onegin; i++){
        value_list[i] = i;
    }

    onp.string_list = str_list;
    onp.value_list = value_list;
    onp.len_lists = strings_onegin;
    return onp;
}

int main(){
    FILE *oneginfp;
    oneginfp=fopen("onegin.txt", "r");
    assert(oneginfp != NULL);

    onegin_params onp = onegin_input(oneginfp);
    onp.value_list = sort_func(&onp);
    onegin_output(&onp);
    return 0;
}