#!/usr/bin/env bash

class=$1

mkdir ./OUTPUT/$class

cd ./$class
array=(*/)
cd ..

# IFS='/' read -ra ADDR <<< "${array[@]}"
# for i in "${ADDR[@]}"; do
#     # process "$i"
# done

ids=("${array[@]//EventID}")
eventIDs=("${ids[@]///}")

#for dir in "${eventIDs[@]}"; do echo "$dir"; done

echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[0]}"
./Sahara.exe -EventClass ${class} -EventID ${eventIDs[0]}

#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[1]}"
#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[2]}"
#echo "./Sahara.exe -EventClass ${class} -EventID ${eventIDs[3]}"
