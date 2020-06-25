#!/bin/bash -l
#SBATCH --qos=regular
#SBATCH -N 1
#SBATCH -A m1820
#SBATCH -J SAHARA
#SBATCH -t 02:00:00
#SBATCH -L SCRATCH
#SBATCH -C knl,quad,cache

num_of_nodes=1
for (( nodeid=1; nodeid <= $num_of_nodes; nodeid++ ))
do
    srun -N 1 -n 50 python job_MPI_wrapper.py 50 $nodeid &
done
wait
