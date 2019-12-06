 Implemented a parallel, scalable with the number of threads, version of the Fourier Transform
and Fast Fourier Transform algorithms, in C using the Pthread API.
Sequential Fourier Transform implementation and  Scalable parallel Fourier Transform implementation (FT.c) along with Scalable parallel Fast Fourier Transform implementation for N as a power of 2 (FFT.c).


The program will be run in the following way:
1 ./FT inputValues.txt outputValues.txt numThreads
2 ./FFT inputValues.txt outputValues.txt numThreads


The input file format:
1 N // number of values
2 value1 // values are of type double
3 value2
4 ...
5 valueN

The output file format:
1 N // number of complex type values represented as pairs of doubles
2 Re-value1 Img-value1 // values are of type double
3 Re-value2 Img-value2
4 ...
5 Re-valueN Img-valueN

To run them:
1 ./compareOutputs file1 file2
2 ./inputGenerator N file RANDOM_SEED



