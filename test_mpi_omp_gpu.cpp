/*
 *  Small test to check if the toolchain used is CUDA-aware
 *  and supports OpenMP offload features.
 *
 *  Compilation (MPI flags need to be added):
 *    - Clang: clang++ test_mpi_omp.cpp -o test_mpi_omp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda
 *    - GCC  : g++     test_mpi_omp.cpp -o test_mpi_omp -fopenmp
 *    - NVHPC: nvc++   test_mpi_omp.cpp -o test_mpi_omp -mp -target=gpu
 */
#include <mpi.h>
#include <omp.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    // Init MPI
    MPI_Init(&argc, &argv);

    // Basic MPI info
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Test variables
    int n = 512*512*512 / size;
    int niter = 100;

    // One GPU per rank    
    omp_set_default_device(rank % omp_get_num_devices());
    printf("Hello from rank n°%d ! I have the device n°%d. There are %d GPU availables.\n", rank, omp_get_default_device(), omp_get_num_devices());

    // Allocate buffer on the GPU
    int *send_buff = (int*)omp_target_alloc(     n*sizeof(int), omp_get_default_device());
    int *recv_buff = (int*)omp_target_alloc(size*n*sizeof(int), omp_get_default_device());

    #pragma omp target teams distribute parallel for is_device_ptr(send_buff)
    for (int i = 0; i < n; i++) send_buff[i] = rank;

    // MPI Comm. data
    int scount[size], rcount[size];
    int sdispl[size], rdispl[size];
    for (int i = 0; i < size; ++i) {
        scount[i] = n;
        rcount[i] = n;

        sdispl[i] = 0;
        rdispl[i] = i*n;
    }

    // Test MPI_Alltoall
    double start = MPI_Wtime();
    for (int i = 0; i < niter; ++i) {
        MPI_Alltoallv(
            send_buff, scount, sdispl, MPI_INT,
            recv_buff, rcount, rdispl, MPI_INT,
            MPI_COMM_WORLD
        );
    }
    double end = MPI_Wtime();

    // Print total time
    double ttot_max;
    double ttot = (end - start) / (double)niter;
    MPI_Allreduce(&ttot, &ttot_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    if (rank == 0)  printf("Comm. took %f [s]\n", ttot_max);

    // // Sanity check
    // if (rank == 0) {
    //     #pragma omp target is_device_ptr(recv_buff)
    //     for (int i = 0; i < size*n; ++i) {
    //         printf("%d ", recv_buff[i]);
    //     }
    //     printf("\n");
    // }

    // Cleanup
    MPI_Finalize();

    return 0;
}
