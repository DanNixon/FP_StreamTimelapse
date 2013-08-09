#!/bin/bash

cd /home/pi/stream_tl

echo `date -u`

echo "Removing old frame image"
rm s_frame.jpg

echo "Creating image folder if not exist"
mkdir tl_images

echo "Will start in "$1" seconds"
sleep $1

screen -L -d -m -S date_rec date -u

echo "Starting MJPEG server"
screen -L -d -m -S mjpg_stream ./mjpg-streamer/start.sh
echo "Starting capture"
#ARGS: [output folder] [image name (%d is frame number)] [timelapse delay ms] [capture time ms (0 for capture untill SIGTERM)]
screen -L -d -m -S stl_capture ./streaming_timelapse/streaming_timelapse tl_images i%d 5000 0

sleep 2

echo "Ensure 2 screens are found:"
screen -ls

screen_l_count=`screen -ls | wc -l`
if [ $screen_l_count == "5" ]; then
	echo "Capture seems to have started properly"
else
	echo "Capture has not correctly started"

	#UNCOMMENT FOR AUTO RECOVERY
	echo "Performing auto recovery"
	dir_found=0
	i=0
	directory=""

	while [ $dir_found == 0 ]
	do
		if [ -d "recovery"$i ]; then
			(( i++ ))
		else
			dir_found=1
			mkdir recovery$i
		fi
	done
	./recovery.sh "recovery"$i
	sudo reboot

fi

echo "To kill: send SIGTERM to (kill) streaming_timelapse and Ctrl-C on mjpg_stream screen"
