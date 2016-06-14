#include "utils.h"

int shm_id;

void * creat_mmap_reg()
{
    key_t key = ftok("/dev/null", SEED);
    if ((shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    void * shm;
    if ((shm = shmat(shm_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    return shm;
}

void quit(int signum)
{
	shmctl(shm_id, IPC_RMID, 0);
	exit(0);
}

void read_file(const char * filepath) 
{
	FILE * fp = fopen(filepath, "r");
	assert(fp > 0);
	
	int neurons;
	int num = fscanf(fp, "%d", &neurons);
	assert(num == 1);
	assert(neurons <= MAX_NEURONS);

	
	double * shm_ptr = creat_mmap_reg();
	assert (shm_ptr != NULL);

	int size = sizeof(neurons);
	memcpy(shm_ptr, &neurons, size);

	double * data = shm_ptr + size;
	for (int i = 0; i < neurons * neurons; ++i)
		fscanf(fp, "%lf", &data[i]);

	print_matrix(data, neurons);

	fclose(fp);
}


int main(int argc, char *argv[])
{
	char *filepath = NULL;

	int c = getopt(argc, argv, "i:");
	if  (c == 'i')
		filepath = optarg;
	else {
		fprintf(stderr, "Unknown option -%c\n", optopt);
		exit(EXIT_FAILURE);
	}

	printf("filename -> %s\n", filepath);

	read_file(filepath); 
	
	struct sigaction sa;
	sa.sa_handler = quit;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGINT, &sa, NULL);

	pause();

	
	return 0;
}



