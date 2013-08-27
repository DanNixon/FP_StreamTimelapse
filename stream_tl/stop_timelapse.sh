#!/bin/bash

cd /home/pi/stream_tl

rm ./gps.temp

echo "Stopping capture"
sudo killall streaming_timelapse
echo "Stopping MJPEG streamer"
pkill -SIGINT mjpg_streamer
