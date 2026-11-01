#ifndef MATRIX_IO_H
#define MATRIX_IO_H

int readNFromFile(const char* path);

void readMatrixAfterN(const char* path, double* M, int N);

void writeMatrix(const char* path, const double* M, int N);

void multiplyLocal(const double* A_local, const double* B,
                   double* C_local, int rows_local, int N);

#endif