#!/bin/bash -l
#SBATCH -N 1
#SBATCH -p regular
#SBATCH -C knl,quad,cache
#SBATCH -t 
#SBATCH -e pp1.$SLURM_JOB_ID.err
#SBATCH -o pp1.$SLURM_JOB_ID.out
#SBATCH -J "SAHARA_TEST"

cd 
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511299816 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511300933 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511391750 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511393700 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511395746 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511410537 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511416763 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511417446 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511422153 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511425928 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511437604 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511552937 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511630960 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511650143 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511658038 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511886132 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1511996808 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512015332 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512173926 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512173935 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512289876 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512335477 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512366534 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512470947 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512495814 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512750994 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512769616 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512903328 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1512972536 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513012799 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513126133 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513168335 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513243360 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513254215 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513286961 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513331030 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513331033 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513342141 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513342148 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513512538 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1513763215 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1514252634 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1514252642 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1514628482 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1514657236 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1514657259 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1514773608 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1515088783 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1515259493 &
srun  --gres=craynetwork:0 -N 1 -n 1 --cpu_bind=cores ./Sahara.exe -EventClass 0.95MB1.05 -EventID 1515370160 &
wait
