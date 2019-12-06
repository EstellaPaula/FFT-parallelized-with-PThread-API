#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>

typedef double complex cplx;
typedef struct Params{
	cplx* buf;
	cplx* out;
	int n;
	int step;
}Params;

int N;
int P;
int numThreads;
typedef double complex cplx;
cplx* inValues;
cplx* outValues;

void init(char* in)
{
	//open file
	FILE *fptr;
	fptr = fopen(in, "r");
	fscanf(fptr, "%d", &N);
	printf("%d\n", N);
	int i;

	inValues = malloc(sizeof(cplx) * N);
	outValues = malloc(sizeof(cplx) * N);

	double aux;

	for(i = 0; i < N; i++) {
		fscanf(fptr, "%lf \n", &aux);
		outValues[i] = inValues[i] = aux;
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
	for(i = 0; i < N; i++) {
			fprintf(fptr, "%lf %lf\n",creal(inValues[i]), cimag(inValues[i]));
	}
	fclose(fptr);
}

void* fftThreadFunction(void *var)
{
	Params p = *(Params*) var;
	int step = p.step;
	//printf("thread step %d \n", step);
	int n = p.n;
	//printf("thread n %d \n", n);
	cplx* buf = p.buf;
	cplx* out = p.out;
	if(step < N) {
		if (step < P) {
			//fftThreadFunction(out, buf, n, step * 2);
			//fftThreadFunction(out + step, buf + step, n, step * 2);
			pthread_t threadRec;
			Params* parameters = (Params*)calloc(1,sizeof(Params)); 
			parameters->buf = out + step;
			parameters->out = buf + step;
			parameters->n = N;
			parameters->step = step * 2;
			pthread_create(&(threadRec), NULL, fftThreadFunction, &(parameters[0]));
			

			Params* parametersLeft = (Params*)calloc(1,sizeof(Params));
			parametersLeft->buf = out;
			parametersLeft->out = buf;
			parametersLeft->n = N;
			parametersLeft->step = step * 2;
			fftThreadFunction(&parametersLeft[0]);

			pthread_join(threadRec, NULL);
			
		} else {
			Params* parametersLeft = (Params*)calloc(1,sizeof(Params));
			parametersLeft->buf = out;
			parametersLeft->out = buf;
			parametersLeft->n = N;
			parametersLeft->step = step * 2;
			fftThreadFunction(&parametersLeft[0]);

			Params* parametersRight = (Params*)calloc(1,sizeof(Params)); 
			parametersRight->buf = out + step;
			parametersRight->out = buf + step;
			parametersRight->n = N;
			parametersRight->step = step * 2;
			fftThreadFunction(&parametersRight[0]);
			
		}
		
		for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * M_PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}

int main(int argc, char * argv[]) {

	//receive arguments
	numThreads = atoi(argv[3]);		//1, 2 sau 4 thread uri
	P = numThreads;
	char* in = argv[1];
	char* out = argv[2];
	init(in);

	pthread_t mainThread;
	struct Params* initParams = (struct Params*)calloc(1,sizeof(struct Params)); 
	initParams->buf = inValues;
	initParams->out = outValues;
	initParams->n = N;
	initParams->step = 1;
	/*printf("%p \n", initParams->buf);
	printf("%p \n", inValues);
	printf("%p \n", initParams->out);
	printf("%p \n", outValues);
	printf("%d \n", initParams->n);
	printf("%d \n", N);
	printf("%d \n", initParams->step);*/

	pthread_create(&(mainThread), NULL, fftThreadFunction, &(initParams[0]));
	pthread_join(mainThread, NULL);

	writeOut(out);

	free(inValues);
	free(outValues);
	//free(initParams);
	return 0;
}
