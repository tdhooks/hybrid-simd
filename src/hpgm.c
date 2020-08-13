/*
*   Inclusions
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hpgm.h"

/*
*   Header Definitions
*/

struct pgm_descriptor *hpgm_read(char *filepath)
{
        char magic_number[10];
        struct pgm_descriptor *pgm;

        if (!(pgm = malloc(sizeof(struct pgm_descriptor))))
                return NULL;

        FILE *file = fopen(filepath, "rb");

        if (file == NULL)
                return NULL;

        fscanf(file, "%s", magic_number);

        fscanf(file, "%d", &pgm->width);
        fscanf(file, "%d", &pgm->height);
        fscanf(file, "%d", &pgm->max_value);

        if (!(pgm->image_buffer = malloc(sizeof(int) * pgm->width * pgm->height)))
                return NULL;

        if (!strcmp(ASCII_PGM_MAGIC_NUMBER, magic_number)) {
                pgm->type = ASCII_PGM;
                for (int i = 0; i < (pgm->height * pgm->height); ++i)
                        if (!fscanf(file, "%d", &pgm->image_buffer[i]))
                                return NULL;
        } else if (!strcmp(RAW_PGM_MAGIC_NUMBER, magic_number)) {
                pgm->type = RAW_PGM;
                pgm->data_size_bytes = (pgm->max_value < PGM_DATA_SIZE_THRESHOLD) ? sizeof(char) : sizeof(short);
                fseek(file, sizeof(char)*1UL, SEEK_CUR);
                if (!fread(pgm->image_buffer, pgm->data_size_bytes, (pgm->width * pgm->height), file))
                        return NULL;
        } else {
                return NULL;
        }

        fclose(file);

        return pgm;
}

int hpgm_flush(struct pgm_descriptor *pgm, char *filename)
{
        FILE *file = fopen(filename, "wb");

        if (file == NULL)
                return -1;

        fprintf(file, "%s\n", pgm->type ? RAW_PGM_MAGIC_NUMBER : ASCII_PGM_MAGIC_NUMBER);
        fprintf(file, "%d %d\n", pgm->width, pgm->height);
        fprintf(file, "%d\n", pgm->max_value);

        if (pgm->type == ASCII_PGM)
                for (int i = 0; i < (pgm->height * pgm->width); ++i)
                        fprintf(file, "%d ", pgm->image_buffer[i]);
        else if (pgm->type == RAW_PGM)
                fwrite(pgm->image_buffer, pgm->data_size_bytes, (pgm->width * pgm->height), file);
        else
                return -1;
        
        fclose(file);

        return 0;
}

int hpgm_free(struct pgm_descriptor *pgm)
{
        free(pgm->image_buffer);
        free(pgm);
        return 0;
}
