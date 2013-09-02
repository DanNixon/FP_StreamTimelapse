FP_StreamTimelapse
==================

360 degree timelapse recorder for BubbleScope with MJPEG streaming to run on the RasPi.

Allows capturing of timelapse frames at a given interval and streaming over a MJPG stream during delay period, also see the ```cloud_streaming``` folder for an example of how to stream to multiple clients efficiently using an intermediate server.

Application can run in 3 "modes" depending on the configuration of run variables:

1.	Basic timelapse: application will record a frame after the timelpase period has elapsed and stream during the delay period.
2.	Location recording timelapse: same as basic but will record the location of every timelapse image and save to EXIF.
3.	Location aware timelapse: same as location recording but will only capture a new timelapse image if a certain distance has been covered since the last image was captured

There is also a sample viewer application which can unwrap an MJPG stream from the Pi or cloud streaming server.

Custom handlers for new streaming frames and GPS data can be configured to start using the ```start_timelapse.sh``` script (usually you would want to start it in a new ```screen``` session) and stop using ```stop_timelapse.sh```. They must be configured to watch ```gps.temp``` for GPS data (in format ```[latitude] [longitude] [altitude] [track] [speed] [timestamp]```) and ```s_frame.jpg``` for streaming frames.

ti_calc.py
----------

The ```ti_calc.py``` tool can be used to calculate the size of SD card you will need for a specific timelapse configuration.

e.g. To calculate the SD card you need for a timelapse lasting 7 days, taking a 2000x2000 image every 2 minutes use the following command:

```python tl_calc.py 2000 604800 120```

Which should calculate a minimum SD card size of 16942.72 MB, hence a 32GB card is recommended.

tl_gen.sh
---------

Simple script to generate an MP4 timelapse video.

Requires ffmpeg.

Usage: ```./tl_gen.sh ./equi/img%d_e.jpg timelapse.mp4``` will generate a timeplapse video named "timelaspe.mp4" from images in the "equi" folder with filename numbering img0_e.jpg, img1_e.jpg...