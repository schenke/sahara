#!/usr/bin/env bash

event=$1

scp ./hadronic_afterburner_toolkit/hadronic_afterburner_tools.e ./OUTPUT/$event
scp -r ./hadronic_afterburner_toolkit/EOS ./OUTPUT/$event

cd ./OUTPUT/$event
mkdir results
mv OSCAR.dat ./results
./hadronic_afterburner_tools.e

