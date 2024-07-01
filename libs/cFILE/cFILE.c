/**
 * @file cFILE.c
 * @brief cFILE_t operations module
 * @details This module implements the basic file operations.
 * @date 2024-02-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "cFILE.h"

#include "libs/cLOG/cLOG.h"

cFILE_t *FILE_Init()
{
    cFILE_t *file = (cFILE_t *)malloc(sizeof(cFILE_t));
    if (file != NULL)
    {
        file->name = malloc(30 * sizeof(char));
        file->buffer = NULL;
        file->pointer = NULL;
        file->path = malloc(30 * sizeof(char));

        file->name[0] = '\0';
        file->path[0] = '\0';

        return file;
    }

    log_fatal("Error: Could not allocate memory for cFILE_t");
    exit(EXIT_FAILURE);
}

void FILE_Free(cFILE_t *file)
{
    if (file->name != NULL)
    {
        free(file->name);
    }
    if (file->buffer != NULL)
    {
        free(file->buffer);
    }
    if (file->pointer != NULL)
    {
        fclose(file->pointer);
    }
    if (file->path != NULL)
    {
        free(file->path);
    }
    free(file);
}

bool FILE_Read(cFILE_t *file)
{
    char full_path[100] = {0};

    strcpy(full_path, file->path);
    strcat(full_path, "/");
    strcat(full_path, file->name);
    strcat(full_path, ".");
    strcat(full_path, FILE_Extension_to_String(file->extension));

    file->pointer = fopen(full_path, "rb");
    if (file->pointer == NULL)
    {
        log_error("Error: Unable to open file %s", full_path);
        return false;
    }

    fseek(file->pointer, 0, SEEK_END);
    file->size = ftell(file->pointer);
    fseek(file->pointer, 0, SEEK_SET);

    file->buffer = (char *)malloc(file->size + 1);

    if (fread(file->buffer, 1, file->size, file->pointer) != file->size)
    {
        log_error("Error: Unable to read file %s", full_path);
        return false;
    }

    file->buffer[file->size] = '\0';

    fclose(file->pointer);

    return true;
}

bool FILE_Write(cFILE_t *file, cFILE_opening_mode_t mode)
{
    char full_path[100] = {0};

    strcpy(full_path, file->path);
    strcat(full_path, "/");
    strcat(full_path, file->name);
    strcat(full_path, ".");
    strcat(full_path, FILE_Extension_to_String(file->extension));

    switch (mode)
    {
    case WRITE:
        file->pointer = fopen(full_path, "w");
        break;

    case APPEND:
        file->pointer = fopen(full_path, "a");
        break;

    default:
        break;
    }

    if (file->pointer == NULL)
    {
        log_error("Error: Unable to open file %s", file->name);
        return false;
    }

    fprintf(file->pointer, "%s", file->buffer);

    fclose(file->pointer);

    return true;
}

char *FILE_Extension_to_String(extension_t extension)
{
    switch (extension)
    {
    case DAT:
        return "dat";
    case TXT:
        return "txt";
    case JSON:
        return "json";
    default:
        return false;
    }
}

extension_t FILE_String_to_Extension(char *extension)
{
    if (FILE_String_Compare_Insensitive(extension, "dat") == 0)
    {
        return DAT;
    }
    else if (FILE_String_Compare_Insensitive(extension, "txt") == 0)
    {
        return TXT;
    }
    else if (FILE_String_Compare_Insensitive(extension, "json") == 0)
    {
        return JSON;
    }
    else
    {
        return UNKNOWN;
    }
}

cFILE_t *FILE_Parse_Path(char *full_path)
{
    char *lastSlash = strrchr(full_path, '/');
    char *lastDot = strrchr(full_path, '.');

    if (lastDot == NULL || (lastSlash != NULL && lastSlash > lastDot))
    {
        log_error("Error: Invalid file path");
        return NULL;
    }

    cFILE_t *file = FILE_Init();

    *lastSlash = '\0';
    sprintf(file->path, "%s", full_path);
    *lastSlash = '/';

    *lastDot = '\0';
    sprintf(file->name, "%s", lastSlash + 1);
    *lastDot = '.';

    file->extension = FILE_String_to_Extension(lastDot + 1);

    return file;
}

int FILE_String_Compare_Insensitive(const char *a, const char *b)
{
    int ca, cb;

    do
    {
        ca = *((unsigned char *)a++);
        cb = *((unsigned char *)b++);
        ca = tolower(toupper(ca));
        cb = tolower(toupper(cb));
    } while (ca == cb && ca != '\0');

    return ca - cb;
}