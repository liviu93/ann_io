#ifndef _UTILS_H
#define _UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>

#define SEED 245
#define MAX_NEURONS 50
#define SHM_SIZE (sizeof(int) + (MAX_NEURONS * MAX_NEURONS * sizeof(double)))

void print_vector(double *arr, int size)
{
	if (!arr) return;

	for (int i = 0; i < size; ++i)
		printf("%.2f ", arr[i]);
	printf("\n");
}


void print_matrix(double *arr, int size)
{
	if (!arr) return;

	for (int i = 0; i < size; ++i)
		print_vector(arr+i*size, size);
}


#endif
