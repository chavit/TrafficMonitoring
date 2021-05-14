# TeleNoise: A Network-Noise Layer for Real-Time In-Band Telemetry (Code for Evaluation)
This repository contains the code reproducing the experimental evaluation in the paper "TeleNoise: A Network-Noise Module
for In-Band Real-Time Telemetry"

The evaluations are based on the YAPS simulator(https://github.com/NetSys/simulator).

## Compile: 

`CXX=g++ cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-std=c++14"` then `make`. 

## Run:

`./simulator 1 conf_file.txt > output.ans`, where `conf_file.txt` is a configuration file 

The simulator outputs values of all calculated metrics to the file `output.ans` (see `output_standard.txt` for example). 


## Parameters variation

* `speed` - corresponds to beta, in evaluations we varied `speed` from 10 to 20.
* `queue_size` - the buffers size in bytes (the number of packets multiplied by 1500).

`conf_standard.txt` -- the configuration file for the standard experiment.

`output_standard.txt` -- values of all calculated metrics in the standard experiment.

## Authors

* Vitalii Demianiuk
* Sergey Gorinsky
* Kirill Kogan
