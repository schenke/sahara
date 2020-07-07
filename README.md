# sahara

On NERSC:

./run_sahara_events_NERSC.sh 0.95MB1.05 10
(executable event_class [number_of_runs with 10 oversampled events each])

when done do

./combine_results_into_OSCAR.sh 0.95MB1.05 10

cd OUTPUT
python combine_results_into_hdf5.py 0.95MB1.05

mkdir results
python average_event_spvn_h5.py 0.95MB1.05.h5 results
