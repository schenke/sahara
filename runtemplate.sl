#!/bin/bash -l
#SBATCH -N 74
#SBATCH -q regular
#SBATCH -C knl,quad,cache
#SBATCH -t 01:20:00
#SBATCH -e pp1.$SLURM_JOB_ID.err
#SBATCH -o pp1.$SLURM_JOB_ID.out
#SBATCH -J my_job

cd /scratch/scratchdirs/schenke/CYM/pp/1
