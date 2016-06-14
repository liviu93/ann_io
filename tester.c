#include "utils.h"


void * get_mmap_reg(int * neurons)
{

	int shm_id;
    key_t key = ftok("/dev/null", SEED);
    if ((shm_id = shmget(key, SHM_SIZE, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    void * shm_ptr;
    if ((shm_ptr = shmat(shm_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
	
    int size = sizeof(*neurons);
    memcpy(neurons, shm_ptr, size);

    double * data = shm_ptr;

    return data;
}

#define WR 1
#define RD 0

double sigmoid(double z)
{
	return (1/(1-exp(-z)));
}

double dot(double * a, double * b, int size)
{
	double sum = 0.0;
	for (int i = 0; i < size; ++i)
		sum += a[i] * b[i];
	return sum;
}

void feed(int size, double * weights, 
		double * inputs, double * outputs, double (*activate)(double))
{
	for (int i = 0; i < size; ++i)
		outputs[i] = activate(dot(weights + (i*size), inputs, size));

	print_vector(outputs, size);
}

int main(int argc, char *argv[])
{

	char *filein = NULL;
	char *fileout = NULL;

	int c;
	while ((c = getopt(argc, argv, "i:o:")) != -1)
		switch(c) {
			case 'i':
				filein = optarg;
				break;
			case 'o':
				fileout = optarg;
				break;
			default:
				fprintf(stderr, "Unknown option -%c\n", optopt);
				exit(EXIT_FAILURE);
		}
	
	printf("Input file -> %s\n", filein);
	printf("Output file -> %s\n", fileout);

	int neurons;
	void * data = get_mmap_reg(&neurons);

	print_matrix(data, neurons);
	printf("===============\n");


	FILE * fp[2];
	fp[RD] = fopen(filein, "r");
	fp[WR] = fopen(fileout, "w");
	assert(fp[RD] != NULL);
	assert(fp[WR] != NULL);

	double idata[neurons];
	double odata[neurons];

	int k = 0;
	for (;;) {
		int num = fscanf(fp[RD], "%lf", &idata[k++]);
		if (num != 1) break;

		if (k == neurons) {
			k = 0;
			feed(neurons, data, idata, odata, sigmoid);
			for (int i = 0; i < neurons; ++i)
				fprintf(fp[WR], "%lf ", odata[i]);
			fprintf(fp[WR], "\n");
		}
	}
	
	fclose(fp[RD]);
	fclose(fp[WR]);
    return 0;
}






