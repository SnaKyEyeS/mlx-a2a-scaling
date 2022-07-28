## How to run

Modules:
```bash
# Clear modules, use latest software stack
module purge
module unuse /apps/USE/easybuild/release/latest/modules/all
module use   /apps/USE/easybuild/release/2021.5/modules/all
module use   /apps/USE/easybuild/staging/2021.5/modules/all

# Load modules 
module load Clang/13.0.1-GCCcore-10.3.0-CUDA-11.3.1
module load UCX-CUDA/1.12.1-GCCcore-10.3.0-CUDA-11.3.1
module load FFTW/3.3.10-gompi-2021a
module load HDF5/1.12.1-gompi-2021a
module load OpenMPI/4.1.4-GCC-10.3.0-cudafix
```

Compilation:
```bash
# Link OpenMPI
alias mpic++="clang++ -I/mnt/tier2/apps/USE/easybuild/release/2021.5/software/OpenMPI/4.1.4-GCC-10.3.0-cudafix/include -I/mnt/tier2/apps/USE/easybuild/release/2021.5/software/OpenMPI/4.1.4-GCC-10.3.0-cudafix/include/openmpi -I/apps/USE/easybuild/release/2021.5/software/hwloc/2.7.1-GCCcore-10.3.0/include -I/apps/USE/easybuild/release/2021.5/software/libevent/2.1.12-GCCcore-10.3.0/include -L/mnt/tier2/apps/USE/easybuild/release/2021.5/software/OpenMPI/4.1.4-GCC-10.3.0-cudafix/lib -L/apps/USE/easybuild/release/2021.5/software/hwloc/2.7.1-GCCcore-10.3.0/lib -L/apps/USE/easybuild/release/2021.5/software/libevent/2.1.12-GCCcore-10.3.0/lib -Wl,-rpath -Wl,/mnt/tier2/apps/USE/easybuild/release/2021.5/software/OpenMPI/4.1.4-GCC-10.3.0-cudafix/lib -Wl,-rpath -Wl,/apps/USE/easybuild/release/2021.5/software/hwloc/2.7.1-GCCcore-10.3.0/lib -Wl,-rpath -Wl,/apps/USE/easybuild/release/2021.5/software/libevent/2.1.12-GCCcore-10.3.0/lib -Wl,--enable-new-dtags -lmpi"

# For the GPU (w/ the OpenMP offload flags)
mpic++ test_mpi_omp.cpp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda
```
