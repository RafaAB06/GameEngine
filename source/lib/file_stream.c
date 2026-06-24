#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

#include "file_stream.h"

char *read_all_bytes(const char *path){
    FILE* file = fopen(path, "rb");

    if(!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

int8 read_int8(FILE *file) {
    int8 num = 0;
    fread(&num, sizeof(int8), 1, file);

    return num;
}

int16 read_int16(FILE *file) {
    int16 num = 0;
    fread(&num, sizeof(int16), 1, file);

    return num;
}

int32 read_int32(FILE *file) {
    int32 num = 0;
    fread(&num, sizeof(int32), 1, file);

    return num;
}

int64 read_int64(FILE *file) {
    int64 num = 0;
    fread(&num, sizeof(int64), 1, file);

    return num;
}

float read_single(FILE *file) {
    float num = 0;
    fread(&num, sizeof(float), 1, file);

    return num;
}

double read_double(FILE *file) {
    double num = 0;
    fread(&num, sizeof(double), 1, file);

    return num;
}

void *read_array(FILE *file, int32 item_size, int32 *count){
    int32 c = read_int32(file);

    void *array = malloc(item_size * c);
    fread(array, item_size, c, file);

    *count = c;

    return array;
}

void write_int8(FILE *file, int8 value) {
    fwrite(&value, sizeof(int8), 1, file);
}

void write_int16(FILE *file, int16 value) {
    fwrite(&value, sizeof(int16), 1, file);
}

void write_int32(FILE *file, int32 value) {
    fwrite(&value, sizeof(int32), 1, file);
}

void write_int64(FILE *file, int64 value) {
    fwrite(&value, sizeof(int64), 1, file);
}

void write_single(FILE *file, float value) {
    fwrite(&value, sizeof(float), 1, file);
}

void write_double(FILE *file, double value) {
    fwrite(&value, sizeof(double), 1, file);
}

void write_array(FILE *file, void *array, int32 size, int32 item_size){
    write_int32(file, size);
    fwrite(array, item_size, size, file);
}
