#!/usr/bin/env bash

fjob="./submit_MPI_template.sl"
name="SAHARA_TEST"
ppns=68
walltime=20:00:00

class=$1
runs=$2

for (( i=0; i<$runs; i++ ))
do
 mkdir $i
 mkdir ./$i/OUTPUT
 mkdir ./$i/OUTPUT/$class
done

cd ./EVENTCLASSES/$class
array=(*/)
cd ..
cd ..

events=${#array[@]}

nodes=$((events/ppns+1))

ids=("${array[@]//EventID}")
eventIDs=("${ids[@]///}")

echo "Running" $events "events on" $nodes "nodes" $runs "times."

awk -v runs=$runs -v ppns=$ppns -v nev=$events -v ii=$count -v name=$name -v nod=$nodes -v wt=$walltime -v class=$class -v id=${eventIDs[i]} 'BEGIN{}{\
if ($1=="#SBATCH" && $2=="-t") printf("#SBATCH -t %s\n",wt);\
else if ($1=="#SBATCH" && $2=="-N") printf("#SBATCH -N %d\n",runs);\
else print $0}' $fjob > ./submit_MPI.sl

for (( i=0; i<$runs; i++ ))
do
    cd $i
    scp ~/sahara/Sahara.exe .
    scp $LOCATIONHERWIG/Herwig .
    scp ../job_MPI_wrapper.py .
    
    echo "cd $i" >> ../submit_MPI.sl
    echo "srun -N " $nodes " -n " $events "python ./job_MPI_wrapper.py "$class " 50 " $i " &" >> ../submit_MPI.sl
    echo "cd .." >> ../submit_MPI.sl
    
    for (( j=0; j<$events; j++ ))
    do
	echo "#!/usr/bin/env bash" > ./run$class-$j.sh
	echo "./Sahara.exe -EventClass" $class "-EventID" ${eventIDs[j]} >> ./run$class-$j.sh
	echo "${eventIDs[i]}"
    done
    cd ..
done
echo "wait" >> ./submit_MPI.sl
sbatch submit_MPI.sl




