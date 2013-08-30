#!/bin/bash

export LD_LIBRARY_PATH=./
mjpg_streamer -i "input_file.so -f /var/www/frame/" -o "output_http.so"
