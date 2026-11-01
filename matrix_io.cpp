#include "matrix_io.h"
#include <cstdio>
#include <cstdlib>

int readNFromFile(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        perror(path);
        exit(1);
    }
    int n;
    if (fscanf(f, "%d", &n) != 1) {
        fprintf(stderr, "readNFromFile: нет N в %s\n", path);
        fclose(f);
        exit(1);
    }
    fclose(f);
    return n;
}
void readMatrixAfterN(const char* path, double* M, int N) {
    FILE* f = fopen(path, "r");
    if (!f) {
        perror(path);
        exit(1);
    }
    int n_file;
    if (fscanf(f, "%d", &n_file) != 1 || n_file != N) {
        fprintf(stderr,
                "readMatrixAfterN: N в %s = %d, ожидалось %d\n",
                path, n_file, N);
        fclose(f);
        exit(1);
    }
    for (int i = 0; i < N * N; ++i) {
        if (fscanf(f, "%lf", &M[i]) != 1) {
            fprintf(stderr, "readMatrixAfterN: мало данных в %s (i=%d)\n",
                    path, i);
            fclose(f);
            exit(1);
        }
    }
    fclose(f);
}
void writeMatrix(const char* path, const double* M, int N) {
    FILE* f = fopen(path, "w");
    if (!f) {
        perror(path);
        exit(1);
    }
    fprintf(f, "%d\n", N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fprintf(f, "%.17g ", M[i * N + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
void multiplyLocal(const double* A_local, const double* B,
                   double* C_local, int rows_local, int N) {
    for (int i = 0; i < rows_local; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A_local[i * N + k] * B[k * N + j];
            }
            C_local[i * N + j] = sum;
        }
    }
}