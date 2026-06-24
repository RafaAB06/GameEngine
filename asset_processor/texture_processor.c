#include "texture_processor.h"
#include "stb_image.h"
#include "types.h"

#include <windows.h>
#include <string.h>
#include <stdio.h>

void process_image(char *path){
    struct texture_metadata{
        int32 width, height, channels;
    };
    struct texture_metadata metadata;
    int8 *bytes = stbi_load(path, &metadata.width, &metadata.height, &metadata.channels, 0);

    int32 len = strlen(path);
    char nya[len + 1];
    strcpy(nya, path);
    char *dot = strrchr(path, '.');
    strcpy(dot, ".texture");

    char build_path[512];
    sprintf(build_path, "..\\build\\%s", path);

    FILE *file = fopen(build_path, "wb");
    fwrite(&metadata, sizeof(struct texture_metadata), 1, file);

    int32 byte_count = metadata.width * metadata.height * metadata.channels;
    fwrite(bytes, 1, byte_count, file);
    
    fclose(file);
    stbi_image_free(bytes);

    printf("processed: %s -> %s\n", nya, build_path);
}

void process_textures(){
    char *extensions[5] = {"png", "jpg", "jpeg"};
    int32 i;
    for(i = 0; i<3; i++){
        char *extension = extensions[i];

        char path[512];
        sprintf(path, "textures\\*.%s", extension);

        WIN32_FIND_DATA findData;
        HANDLE hFind = NULL;

        hFind = FindFirstFile(path, &findData);
        if (hFind != INVALID_HANDLE_VALUE){
            do
            {
                char file_path[512];
                sprintf(file_path, "textures\\%s", findData.cFileName);
                process_image(file_path);
            }
            while (FindNextFile(hFind, &findData));

            FindClose(hFind);
        }
    }
}