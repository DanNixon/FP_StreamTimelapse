Install/Config
==============

Installation
------------

1.  Clone repo onto Pi SD card
2.  Set path to stream_tl in ```start_timelapse.sh```, ```stop_timelaspe.sh``` and ```recovery.sh```
3.  Optionally use ```crontab -e -u pi``` to start capture on boot using command and line from ```crontab.txt```

Configure timelapse
-------------------

4.  Configure delay between timelapse captures, this is set in [start_timelapse.sh](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/start_timelapse.sh#L22), by default it is set to 5000ms.


Configure streaming_timelapse
-----------------------------

5.	Configure timelapse camera options ( [main.cpp L17](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/streaming_timelapse/main.cpp#L17) )
6.	Configure MJPG stream camera options ( [main.cpp L18](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/streaming_timelapse/main.cpp#L18) )
7.	Configure misc timelapse options ( [main.cpp L19-20](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/streaming_timelapse/main.cpp#L19) )
	-	min_tl_delay = having a timelapse period too low reduced stream consistency, a minimum helps prevent that
	-	tl_cap_run_in = time camera module is running for before capturing a timelapse image, improves consistency of exposure
8.	Configure equi. image generation ( [main.cpp L42](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/streaming_timelapse/main.cpp#L42) )
	-	Parameters same as general equi. generation

Optional Stuff
--------------

-	Disable Auto Recovery
	-	Auto recovery can get the Pi stuck in a reboot cycle if it fails (although this is very unlikely)
	- It can be disabled by commenting out lines 36-51 of ```start_timelapse.sh```
- Custom web pages
	-	mjpg-streamer has a webserver, it's contents can be modified by changing the contents of the www folder inside mjpg-streamer

Notes
-----

For both 5 and 6, options are passed to the ```raspistill``` command and control the image size and effects/filters.

- -n = no preview
- -h, -w = height and width
- -tl = timelapse interval, in this case this is used to update the ```s_frame.jpg``` file which is used for the MJPEG stream
- Full raspicam/raspistill docs [here](https://github.com/raspberrypi/userland/blob/master/host_applications/linux/apps/raspicam/README.md)
