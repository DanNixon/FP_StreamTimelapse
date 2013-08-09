#!/bin/bash

cd /home/pi/stream_tl

echo "Stopping capture"
sudo killall streaming_timelapse
echo "Stopping MJPEG streamer"
pkill -SIGINT mjpg_streamer
