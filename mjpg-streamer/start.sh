#!/bin/bash

cd mjpg-streamer
export LD_LIBRARY_PATH=./
./mjpg_streamer -i "input_file.so -f ../" -o "output_http.so -w ./www"
