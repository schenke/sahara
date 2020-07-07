#!/bin/bash -l
#SBATCH --qos=regular
#SBATCH -N 1
#SBATCH -A m1820
#SBATCH -J SAHARA
#SBATCH -t 02:00:00
#SBATCH -L SCRATCH
#SBATCH -C knl,quad,cache

num_of_nodes=1
