#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>

int N;
int numThreads;
typedef double complex cplx;
cplx* inValues;
cplx* outValues;
int offset;

void* threadFunction(void *var)
{
	int id = *(int *) var;
	int start = id * offset;
	int end = start + offset;
	if(id == numThreads - 1) {
		end = N; 
	}
	int p,r;
	for(p = start; p < end; p++) {
		outValues[p] = 0;
		for(r = 0; r < N; r++) {
			outValues[p] += inValues[r] * cexp((-2 * M_PI * I * p * r)/ N);
		}
	}
}

void init(char* in)
{
	//open file
	FILE *fptr;
	fptr = fopen(in, "r");
	fscanf(fptr, "%d", &N);
	printf("%d\n", N);
	int i;

	inValues = calloc(N,sizeof(cplx));
	outValues = calloc(N,sizeof(cplx));

	for(i = 0; i < N; i++) {
		fscanf(fptr, "%lf", &inValues[i]);
	}
	fclose(fptr);
}

void writeOut(char* out)
{
	//open file
	FILE *fptr;
	fptr = fopen(out, "w");
	int i;

	fprintf(fptr, "%d\n", N);
	for(i = 0; i < N; i++){
		if (!cimag(outValues[i]))
			fprintf(fptr, "%lf %d ", creal(outValues[i]), 0);
		else
			fprintf(fptr, "%lf %lf ", creal(outValues[i]),cimag(outValues[i]));
		if(i != N-1){
		fprintf(fptr, "\n");
		}
	}

	fclose(fptr);
}

int main(int argc, char * argv[]) {

	//receive arguments
	numThreads = atoi(argv[3]);		//1, 2 sau 4 thread uri
	char* in = argv[1];
	char* out = argv[2];

	int i, k;
	init(in);					//initialize vector of values (read from file)
	pthread_t tid[numThreads];
	int id_p[numThreads];
		
	offset =  N/numThreads;

	for(i = 0; i < numThreads; i++) {
		id_p[i] = i;
		pthread_create(&(tid[i]), NULL, threadFunction, &(id_p[i]));
	}

	for(i = 0; i < numThreads; i++) {
		pthread_join(tid[i], NULL);
	}

	writeOut(out);

	free(inValues);
	free(outValues);
	return 0;
}
