#!/usr/bin/env bash

class=$1
runs=$2

mkdir ./OUTPUT
mkdir ./OUTPUT/$class

cd ./EVENTCLASSES/$class
array=(*/)
cd ..
cd ..

events=${#array[@]}

ids=("${array[@]//EventID}")
eventIDs=("${ids[@]///}")

echo "Merging OSCAR files from" $runs "runs."
    
for (( j=0; j<$events; j++ ))
do
    echo ${eventIDs[j]}
    mkdir ./OUTPUT/$class/${eventIDs[j]}
    rm ./OUTPUT/$class/${eventIDs[j]}/OSCAR.dat
    touch ./OUTPUT/$class/${eventIDs[j]}/OSCAR.dat
    
    for (( i=0; i<$runs; i++ ))
    do
	if [ $i == 0 ]
	then
	   cat ./$i/OUTPUT/$class/${eventIDs[j]}/OSCAR.dat >> ./OUTPUT/$class/${eventIDs[j]}/OSCAR.dat
	else
	    tail -n +4 ./$i/OUTPUT/$class/${eventIDs[j]}/OSCAR.dat >> ./OUTPUT/$class/${eventIDs[j]}/OSCAR.dat
	fi
    done

    ./run_toolkit.sh $class ${eventIDs[j]}
done




