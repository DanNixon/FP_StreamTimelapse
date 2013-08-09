Installation
============

1.  Clone repo onto Pi SD card
2.  Set path to stream_tl in ```start_timelapse.sh```, ```stop_timelaspe.sh``` and ```recovery.sh```
3.  Optionally use ```crontab -e -u pi``` to start capture on boot using command and line from ```crontab.txt```
4.  Configure delay between timelapse captures, this is set in [start_timelapse.sh](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/start_timelapse.sh#L22), by default it is set to 5000ms.
5.  Configure capture options, see below.


Configure streaming_timelapse
-----------------------------

1.	Configure timelapse camera options ( [main.cpp L17](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/streaming_timelapse/main.cpp#L17) )


For both 1 and 2, options are passed to the ```raspistill``` command and control the image size and effects/filters.
-n = no preview
-h, -w = height and width
-tl = timelapse interval, in this case this is used to update the ```s_frame.jpg``` file which is used for the MJPEG stream