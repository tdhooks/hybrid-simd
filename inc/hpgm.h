#ifndef _HPGM_H_
#define _HPGM_H_

/*
*   Macros
*/

#define PGM_DATA_SIZE_THRESHOLD 256
#define ASCII_PGM_MAGIC_NUMBER "P2"
#define RAW_PGM_MAGIC_NUMBER "P5"
#define ASCII_PGM 0
#define RAW_PGM 1
/*
*   Image struct
*/

struct pgm_descriptor {
        int type;
        int width;
        int height;
        int max_value;
        int data_size_bytes;
        int *image_buffer;
};

/*
*   Signatures
*/

struct pgm_descriptor *hpgm_read(char *filepath);
int hpgm_flush(struct pgm_descriptor *pgm, char *filename);
int hpgm_free(struct pgm_descriptor *pgm);

#endif
