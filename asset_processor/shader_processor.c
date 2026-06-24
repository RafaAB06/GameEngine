#include "shader_processor.h"
#include "types.h"

#include <windows.h>
#include <stdio.h>

void parse_shader(char *path){
    FILE *glsl_file = fopen(path, "rb");

    fseek(glsl_file, 0L, SEEK_END);
    int32 byte_count = ftell(glsl_file);
    rewind(glsl_file);

    char build_path[512];
    sprintf(build_path, "..\\build\\%s", path);

    char *dot = strrchr(build_path, '.');
    strcpy(dot, ".shader");

    FILE *shader_file = fopen(build_path, "wb");
    char *content = malloc(byte_count + 1);
    fread(content, 1, byte_count, glsl_file);
    fwrite(content, 1, byte_count, shader_file);
    content[byte_count] = 0;

    free(content);
    fclose(glsl_file);
    fclose(shader_file);

    printf("processed: %s -> %s\n", path, build_path);
}

void process_shaders(){
    char path[512] = "shaders\\*.glsl";

    WIN32_FIND_DATA findData;
    HANDLE hFind = NULL;

    hFind = FindFirstFile(path, &findData);
    if (hFind != INVALID_HANDLE_VALUE){
        do
        {
            char file_path[512];
            sprintf(file_path, "shaders\\%s", findData.cFileName);
            parse_shader(file_path);
        }
        while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }
}