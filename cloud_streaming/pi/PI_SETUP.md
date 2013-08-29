Raspi Setup
===========

Setup the image uploader on the Pi

1.	Copy ```imageWatcher.py``` to the ```stream_tl``` folder on the Pi
2.	Change the following in ```start_timelapse.sh```:
	-	```screen -L -d -m -S mjpg_stream ./mjpg-streamer/start.sh``` to
	-	```screen -L -d -m -S mjpg_stream python ./imageWatcher.py```
3.	Append the following lines to ```stop_timelapse.sh```:
	-	```echo "Removing queued streaming frames"```
	-	```rm frame*.jpg```
