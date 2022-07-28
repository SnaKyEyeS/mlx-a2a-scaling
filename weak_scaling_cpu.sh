#!/bin/bash -l

## Launch the jobs
export version="cpu"
ngpus=(1 2 4 8 16 32 64)

for ngpu in ${ngpus[@]}
do
    # Number of nodes needed
    export nnode=$(( ${ngpu} / 4 ))
    if [ ${nnode} -eq 0 ]
    then
        nnode=1
    fi

    # Number of tasks
    ntasks=${ngpu}
    ntasks_per_node=$(( ${ntasks} / ${nnode} ))

    # Run
    echo "Running with..."
    echo "  - ${ngpu} gpus"
    echo "  - ${nnode} nodes"
    echo "  - ${ntasks} tasks, ${ntasks_per_node} per node"

    sbatch  --job-name=test_${ngpu}gpus \
            --nodes=${nnode} \
            --ntasks=${ntasks} \
            --ntasks-per-node=${ntasks_per_node} \
            --time=00:15:00 \
            --partition=cpu \
            --account=p200067 \
            --qos=default \
            kernel.sh
done
