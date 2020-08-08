/*
*   Inclusions
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <mpi.h>

/*
*   Macros and constants
*/

const int COMM_ROOT_RANK = 0;
const int DATA_SIZE_THRESHOLD = 256;

const char *ASCII_PGM_MAGIC_NUMBER = "P2";
const char *RAW_PGM_MAGIC_NUMBER = "P5";

/*
*   Image struct
*/

struct image_descriptor {
        int width;
        int height;
        int max_value;
        int data_size_bytes;
        int *image_buffer;
};

/*
*   Globals
*/

FILE *current_file;

/*
*   Functions
*/

struct image_descriptor *load_pgm(char *filepath);
void manipulate_pgm(struct image_descriptor *image);
void output_pgm(struct image_descriptor *image, char *filename);

void teardown(struct image_descriptor *image);

/*
*   Logic
*/

int main(int argc, char **argv)
{
        if (argc != 3) {
                printf("Usage: ./hybrid_image <image-file-path> <output-image-name>\n");
                exit(1);
        }

        struct image_descriptor *image;

        int rank, size;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        printf("MPI node initialized for node %d of %d\n", rank + 1, size);

        if (rank == COMM_ROOT_RANK) {
                image = load_pgm(argv[1]);
        }

        manipulate_pgm(image);

        if (rank == COMM_ROOT_RANK) {
                output_pgm(image, argv[2]);
                printf("Done.\n");
        }

        MPI_Finalize();

        teardown(image);
}

struct image_descriptor *load_pgm(char *filepath)
{
        int i, j;
        char magic_number[10];
        struct image_descriptor *image;

        if (!(image = malloc(sizeof(struct image_descriptor)))) {
                printf("Failed to initilize image descriptor.\n");
                exit(1);
        }

        current_file = fopen(filepath, "rb");
        if (current_file == NULL) {
                printf("Failed to open image input.\n");
                exit(1);
        }

        fscanf(current_file, "%s", magic_number);

        fscanf(current_file, "%d", &image->width);
        fscanf(current_file, "%d", &image->height);
        fscanf(current_file, "%d", &image->max_value);

        if (!(image->image_buffer = malloc(sizeof(int) * image->width * image->height))) {
                printf("Failed to initialize image buffer.\n");
                exit(1);
        }

        image->data_size_bytes = (image->max_value < DATA_SIZE_THRESHOLD) ? sizeof(char) : sizeof(short);

        if (strcmp(ASCII_PGM_MAGIC_NUMBER, magic_number) == 0) {
                for (i = 0; i < image->height; ++i) {
                        for (j = 0; j < image->width; ++j) {
                                if (!fscanf(current_file, "%d", &image->image_buffer[i * image->width + j])) {
                                        printf("Error loading image buffer.\n");
                                        exit(1);
                                }
                        }
                }
        } else if (strcmp(RAW_PGM_MAGIC_NUMBER, magic_number) == 0) {
                for (i = 0; i < image->height; ++i) {
                        for (j = 0; j < image->width; ++j) {
                                if (!fread(&image->image_buffer[i * image->width + j], image->data_size_bytes, 1, current_file)) {
                                        printf("Error loading image buffer.\n");
                                        exit(1);
                                }
                        }
                }
        } else {
                printf("Invalid image format, must be .pgm.\n");
                exit(1);
        }

        fclose(current_file);

        return image;
}

void manipulate_pgm(struct image_descriptor *image)
{
#pragma omp parallel for shared(image) collapse(2)
        for (int i = 0; i < image->height; ++i) {
                for (int j = 0; j < image->width; ++j) {
                        image->image_buffer[i * image->width + j] = image->max_value - image->image_buffer[i * image->width + j];
                }
        }
}

void output_pgm(struct image_descriptor *image, char *filename)
{
        int i, j;
        char filepath[1024] = "./output/";

        current_file = fopen(strcat(filepath, filename), "wb");
        if (current_file == NULL) {
                printf("Failed to open image output.\n");
                exit(1);
        }

        fprintf(current_file, "%s\n", RAW_PGM_MAGIC_NUMBER);
        fprintf(current_file, "%d %d\n", image->width, image->height);
        fprintf(current_file, "%d\n", image->max_value);

        for (i = 0; i < image->height; ++i) {
                for (j = 0; j < image->width; ++j) {
                        fwrite(&image->image_buffer[i * image->width + j], image->data_size_bytes, 1, current_file);
                }
        }

        fclose(current_file);
}

void teardown(struct image_descriptor *image)
{
        free(image->image_buffer);
        free(image);
}
