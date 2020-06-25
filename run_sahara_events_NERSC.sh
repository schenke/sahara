#!/usr/bin/env bash

fjob="./runtemplate.sl"
name="SAHARA_TEST"
ppns=68
walltime=00:30:00

class=$1

mkdir ./OUTPUT/$class

cd ./EVENTCLASSES/$class
array=(*/)
cd ..
cd ..

events=${#array[@]}

nodes=$((events/ppns+1))

# a=$((totalcpus-nodes*ppns))
# if [ "$a" -gt "0" ]; then
#     nodes+=1;
# fi

echo "Running" $events "events on" $nodes "nodes."
# IFS='/' read -ra ADDR <<< "${array[@]}"
# for i in "${ADDR[@]}"; do
#     # process "$i"
# done

ids=("${array[@]//EventID}")
eventIDs=("${ids[@]///}")

scp ~/sahara/Sahara.exe .
scp $LOCATIONHERWIG/Herwig .

for (( i=0; i<$events; i++ ))
do
    # awk -v ii=$count -v name=$name -v nod=$nodes -v wt=$walltime -v id=${eventIDs[i]} 'BEGIN{}{\
    # if($1=="#SBATCH" && $2=="-J") printf("#SBATCH -J \"%s-%d\"\n",name,id);\
    # else if($1== "#SBATCH" && $2=="-N")\
    # printf("#SBATCH -N %i\n",nod);\
    # else if($1== "#SBATCH" && $2=="-t" && $3=="01:20:00")\
    # printf("#SBATCH -t %s\n",wt);\
    # else if($1== "cd") printf("\n");\
    # else if($1== "#SBATCH" && $2=="-q") printf("#SBATCH -p debug\n");\
    # else print $0}' $fjob > ./run${eventIDs[i]}.sl
    echo "#!/usr/bin/env bash" > ./run$i.sh
    echo "./Sahara.exe -EventClass" $class "-EventID" ${eventIDs[i]} >> ./run$i.sh
    echo "${eventIDs[i]}"
done

sbatch submit_MPI.sl 

