#ifndef CFILE_H
#define CFILE_H

#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    UNKNOWN = -1,
    DAT,
    TXT,
    JSON,
} extension_t;

typedef enum
{
    WRITE = 'w',
    APPEND = 'a'
} cFILE_opening_mode_t;

typedef struct
{
    size_t size;
    char *name;
    char *buffer;
    FILE *pointer;
    char *path;
    extension_t extension;
} cFILE_t;

cFILE_t *FILE_Init();

void FILE_Free(cFILE_t *file);

bool FILE_Read(cFILE_t *file);

bool FILE_Write(cFILE_t *file, cFILE_opening_mode_t mode);

char *FILE_Extension_to_String(extension_t extension);

extension_t FILE_String_to_Extension(char *extension);

cFILE_t *FILE_Parse_Path(char *full_path);

int FILE_String_Compare_Insensitive(const char *a, const char *b);

#endif