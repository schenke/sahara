#!/usr/bin/env bash

fjob="./runtemplate.sl"
name="SAHARA_TEST"
ppns=68

class=$1

mkdir ./OUTPUT/$class

cd ./$class
array=(*/)
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


awk -v ii=$count -v name=$name -v nod=$nodes 'BEGIN{}{\
if($1=="#SBATCH" && $2=="-J") printf("#SBATCH -J \"%s\"\n",name);\
else if($1== "#SBATCH" && $2=="-N")\
printf("#SBATCH -N %i\n",nod);\
else if($1== "#SBATCH" && $2=="-t" && $3=="01:20:00")\
printf("#SBATCH -t %s\n",wt);\
else if($1== "cd") printf("cd %s%s\n",ddd,sss);\
else if($1== "#SBATCH" && $2=="-q") printf("#SBATCH -p regular\n");\
else print $0}' $fjob > ./run.sl

for (( i=0; i<$events; i++ ))
do
    echo "srun  --gres=craynetwork:0 -N" 1 "-n" 1 "--cpu_bind=cores ./Sahara.exe -EventClass" $class "-EventID" ${eventIDs[i]} "&" >> ./run.sl
    echo "${eventIDs[i]}"
done

echo "wait" >> ./run.sl

#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[0]}"
#./Sahara.exe -EventClass ${class} -EventID ${eventIDs[0]}

#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[1]}"
#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[2]}"
#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[3]}"
