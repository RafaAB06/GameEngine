#ifndef IO_H
#define IO_H

#include <stdio.h>

#include "types.h"

char *read_all_bytes(const char *path);

int8 read_int8(FILE *file);
int16 read_int16(FILE *file);
int32 read_int32(FILE *file);
int64 read_int64(FILE *file);

void *read_array(FILE *file, int32 item_size, int32 *count);

float read_single(FILE *file);
double read_double(FILE *file);

void write_int8(FILE *file, int8 value);
void write_int16(FILE *file, int16 value);
void write_int32(FILE *file, int32 value);
void write_int64(FILE *file, int64 value);

void write_single(FILE *file, float value);
void write_double(FILE *file, double value);

void write_array(FILE *file, void *array, int32 size, int32 item_size);

#endif