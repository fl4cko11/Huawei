#include "onegin_ctors.h"

int strs_in_buffer(char *buffer) {
    assert(buffer != nullptr);

    int str_count = 0;
    for (int j = 0; buffer[j] != '\0'; j++) {
        if (buffer[j] == '\n') {
            str_count++;
        }
    }

    return str_count + 1; // Возвращаем количество строк и одну для null
}

buffer_params buffer_Ctor(const char* pathname) {
    assert(pathname != nullptr);
    buffer_params bp = {};

    FILE *fp = fopen(pathname, "rb");
    if (!fp) {
        perror("No удалось открыть файл");
        exit(-1);
    }

    if (fseek(fp, 0, SEEK_END) == -1) {
        printf("fseek error");
        fclose(fp);
        exit(-1);
    };
    long fileSize = ftell(fp); // получаем текущую позицию (размер файла)
    if (fseek(fp, 0, SEEK_SET) == -1) { // возвращаем указатель в начало файла
        printf("fseek error");
        fclose(fp);
        exit(-1);
    }

    // Выделяем память для буфера
    bp.buffer = (char *)calloc(fileSize + 1, sizeof(char *)); // +1 для завершающего нуля
    if (!bp.buffer) {
        perror("No удалось выделить память");
        fclose(fp);
        exit(-1);
    }

    // Читаем файл в буфер
    size_t bytes_read = fread(bp.buffer, 1, fileSize, fp);
    if (bytes_read < fileSize) {
        if (feof(fp)) {
            printf("End of file reached. Read %zu elements.\n", bytes_read);
        } 
        else if (ferror(fp)) {
            perror("Error reading from file");
            fclose(fp);
            exit(-1);
        }
    }
    bp.buffer[bytes_read] = '\0'; // добавляем завершающий ноль для строк

    bp.strs_in_buffer = strs_in_buffer(bp.buffer);

    // Закрываем файл
    fclose(fp);
    return bp;
}

str_ptrs *struct_ptrs_array_Ctor(buffer_params *bp) {
    assert(bp != nullptr);

    str_ptrs *str_ptrs_ar = (str_ptrs *)calloc(bp->strs_in_buffer, sizeof(str_ptrs));
    if (str_ptrs_ar == NULL) {
        printf("calloc error!");
        exit(-1);
    }

    int i = 0;
    int for_last_str = 0;

    str_ptrs_ar[i].str_start = &bp->buffer[0];
    for (int j = 0; bp->buffer[j] != '\0'; j++) {
        for_last_str = j;
        if (bp->buffer[j] == '\n') {
            str_ptrs_ar[i].str_end = &bp->buffer[j-1];
            if (bp->buffer[j+1] != '\0') {
                str_ptrs_ar[i+1].str_start = &bp->buffer[j+1];
            }
            i++;
        }
    }

    // обработка если последняя строка без /n
    if (bp->buffer[for_last_str] != '\n') {
        str_ptrs_ar[i].str_end = &bp->buffer[for_last_str];
    }
    return str_ptrs_ar;
}