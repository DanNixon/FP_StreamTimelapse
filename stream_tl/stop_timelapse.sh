#!/bin/bash

##Expect this to throw out errors or usage docs if called without a (complete) timelapse running

cd /home/pi/stream_tl

rm ./gps.temp

echo "Stopping capture"
sudo killall streaming_timelapse

echo "Stopping MJPEG streamer"
pkill -SIGINT mjpg_streamer

echo "Killing GPS updater"
sudo kill $(ps h --ppid $(screen -ls | grep gps | cut -d. -f1) -o pid)

echo "Killing GPIO watcher"
sudo kill $(ps h --ppid $(screen -ls | grep button_watcher | cut -d. -f1) -o pid)
