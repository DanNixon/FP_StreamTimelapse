Raspi Setup
===========

Setup the image uploader on the Pi

1.	Copy ```imageWatcher.py```, ```tlUpload.py``` and ```upload_frame.sh``` to the ```stream_tl``` folder on the Pi
2.	Change the following in ```start_timelapse.sh```:
	-	```screen -L -d -m -S mjpg_stream ./mjpg-streamer/start.sh``` to
	-	```screen -L -d -m -S mjpg_stream python ./imageWatcher.py```
3.	Append the following lines to ```stop_timelapse.sh```:
	-	```echo "Killing frame uploader"```
	-	```sudo kill $(ps h --ppid $(screen -ls | grep mjpg_stream | cut -d. -f1) -o pid)```
	-	```echo "Removing queued streaming frames"```
	-	```rm frame*.jpg```
4.	Append the following to the "starting capture" ```screen``` command in ```start_timelapse.sh```: ``` "./upload_frame.sh %s"```