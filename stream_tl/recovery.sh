#!/bin/sh

cd /home/pi/stream_tl

if [ -z "$1" ]
then
echo "Timelapse recovery tool"
echo "Use when start_timelapse.sh fails to start capture"
echo "Usage: recovery.sh [path to store recovered files]"
exit 1
fi

echo "Ensuring timelapse is stopped"
./stop_timelapse.sh

echo "Making recovery folder"
mkdir $1

echo "Moving log files"
mv ./screenlog.0 $1
mv ./cron_tl.log $1

echo "Moving images"
mv ./tl_images $1

cd ./streaming_timelapse

echo "Removing capture app files"
rm -r CMakeFiles/
rm Makefile
rm CMakeCache.txt
rm cmake_install.cmake
rm streaming_timelapse

echo "Compiling capture app"
cmake .
make

echo "Done"
