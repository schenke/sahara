#!/usr/bin/env bash

class=$1
cd ../EVENTCLASSES/$class
array=(*/)
cd ..
cd HERWIGCLUSTERS

events=${#array[@]}

echo "Have" $events "."
# IFS='/' read -ra ADDR <<< "${array[@]}"
# for i in "${ADDR[@]}"; do
#     # process "$i"
# done

ids=("${array[@]//EventID}")
eventIDs=("${ids[@]///}")

counter=43

for (( i=0; i<$events; i++ ))
do
    cd /project/projectdirs/m1820/SAHARA/OUTPUT/${class}/${eventIDs[i]}/TMP
    clusters=(*.log)
    ncluster=${#clusters[@]}
    for (( j=0; j<$ncluster; j++ ))
    do
	ls ${clusters[j]}
	scp ${clusters[j]} /project/projectdirs/m1820/SAHARA/HERWIGCLUSTERS/Cluster_HERWIGOutput_${counter}.log
	counter=$(($counter+1))
    done
    #    echo "./Sahara.exe -EventClass" $class "-EventID" ${eventIDs[i]} >> ./run$class-$i.sh
#    echo "${eventIDs[i]}"
done

