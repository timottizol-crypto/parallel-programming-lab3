#include <iomanip>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int readNFromFile(const char* path){
    ifstream in(path);    
    if (!in.is_open()) {
        cerr << "Не открыт файл: " << path << endl;
        exit(1);
    }
    int N = 0;
    in >> N;
    if(!in){
        cerr << "Нет числа N в " << path << endl;
        exit(1);
    }
    return N;
}
void readMatrixAfterN(const char* path, double* M , int N){
   ifstream in(path);
    if (!in.is_open()) {
        cerr << "Не открыт файл: " << path << endl;
        exit(1);
    }
    int n_file = 0;
    in >> n_file;
    if (!in || n_file != N) {
        cerr << "N в файле " << path << " не совпадает с ожидаемым " << N << endl;
        exit(1);
    }
    for (int i = 0; i < N * N; ++i) {
        in >> M[i];
        if (!in) {
            cerr << "Мало данных в " << path << endl;
            exit(1);
        }
    }
}
void writeMatrix(const char* path , const double* M , int N){
    ofstream out(path);
    if (!out.is_open()) {
        cerr << "Не открыт файл на запись: " << path << endl;
        exit(1);
    }
    out << N << "\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            out << std::setprecision(15) << M[i * N + j] << " "; 
        }
        out << "\n";
    }
    out.close();
}
void multiplyLocal(const double* A_local ,const double* B,
                    double* C_local,int rows_local,int N){
    for (int i = 0; i < rows_local ; i++){
        for(int j = 0; j < N ; j++){
            double sum = 0.0;
            for(int k = 0; k < N ; k++){
                sum += A_local[i*N + k] * B[k*N + j];
            }
            C_local[i * N + j] = sum;
        }
    }

}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (argc < 3) {
        if (rank == 0)
            cerr << "Использование: " << argv[0] << " A.txt B.txt\n";
        MPI_Finalize();
        return 1;
    }
    int N = 0;
    if (rank == 0) N = readNFromFile(argv[1]);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (N % size != 0) {
        if (rank == 0) fprintf(stderr, "N=%d не делится на size=%d\n", N, size);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int rows_local = N / size;
    double* A_local = new double[rows_local * N];
    double* B       = new double[N * N];
    double* C_local = new double[rows_local * N];
    double *A = nullptr, *C = nullptr;
    if (rank == 0) { A = new double[N * N]; C = new double[N * N]; }
    if (rank == 0) {
        readMatrixAfterN(argv[1], A, N);
        readMatrixAfterN(argv[2], B, N);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    MPI_Scatter(A, rows_local * N, MPI_DOUBLE,
                A_local, rows_local * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    multiplyLocal(A_local, B, C_local, rows_local, N);
    MPI_Gather(C_local, rows_local * N, MPI_DOUBLE,
               C,       rows_local * N, MPI_DOUBLE,
               0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    if (rank == 0) printf("N=%d, P=%d, time=%.6f\n", N, size, t1 - t0);
    if (rank == 0) writeMatrix("C_mpi.txt", C, N);
    delete[] A_local; delete[] B; delete[] C_local;
    if (rank == 0) { delete[] A; delete[] C; }
    MPI_Finalize();
    return 0;
}