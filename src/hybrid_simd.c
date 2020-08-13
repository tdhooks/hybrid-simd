/*
*   Inclusions
*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <mpi.h>

#include "hpgm.h"

/*
*   Constants and Macros
*/

#define CHECKED(X) if(X) { printf("Runtime Failure.\n"); exit(1); }
#define CHECK_EXISTS(X) if(!X) { printf("Structure load failure.\n"); exit(1); }

const int COMM_ROOT_RANK = 0;

/*
*   Functions
*/

int manipulate_pgm(int *buffer, int buf_size, int max_value);

/*
*   Logic
*/

int main(int argc, char **argv)
{
        if (argc != 3) {
                printf("Usage: ./hybrid_image <image-file-path> <output-image>\n");
                exit(1);
        }

        int rank, size;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        if (rank == COMM_ROOT_RANK) {
                struct pgm_descriptor *image = NULL;
                image = hpgm_read(argv[1]);
                CHECK_EXISTS(image);
                CHECKED(manipulate_pgm(image->image_buffer, (image->width * image->height), image->max_value));
                CHECKED(hpgm_flush(image, argv[2]));
                CHECKED(hpgm_free(image));
                printf("Done.\n");
        }

        MPI_Finalize();
}

int manipulate_pgm(int *buffer, int buf_size, int max_value)
{
        #pragma omp parallel for default(shared)
        for (int i = 0; i < buf_size; ++i)
                buffer[i] = max_value - buffer[i];

        return 0;
}
