CC=mpicc
CFLAGS=-Wall -Wextra -fopenmp

hybrid_simd: hybrid_simd.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm hybrid_simd

.PHONY: clean_output
clean_output: 
	rm ./output/*	