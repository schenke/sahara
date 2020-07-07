#!/usr/bin/env bash

class=$1
event=$2

scp ./hadronic_afterburner_toolkit/hadronic_afterburner_tools.e ./OUTPUT/$class/$event
scp ./hadronic_afterburner_toolkit/parameters.dat ./OUTPUT/$class/$event
scp -r ./hadronic_afterburner_toolkit/EOS ./OUTPUT/$class/$event
#scp ./hadronic_afterburner_toolkit/ebe_scripts/combine_results_into_hdf5.py ./OUTPUT/$class

cd ./OUTPUT/$class/$event
mkdir results
scp OSCAR.dat ./results/OSCAR.DAT
./hadronic_afterburner_tools.e
mv ./results/OSCAR.DAT .
#cd ../..
#python combine_results_into_hdf5.py $event

