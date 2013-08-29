FP_StreamTimelapse
==================

360 degree timelapse recorder for BubbleScope with MJPEG streaming to run on the RasPi.

Allows capturing of timelapse frames at a given interval and streaming over a MJPG stream during delay period, also see the ```cloud_streaming``` folder for an example of how to stream to multiple clients efficiently using an intermediate server.

Application can run in 3 "modes" depending on the configuration of run variables:

1.	Basic timelapse: application will record a frame after the timelpase period has elapsed and stream during the delay period.
2.	Location recording timelapse: same as basic but will record the location of every timelapse image and save to EXIF.
3.	Location aware timelapse: same as location recording but will only capture a new timelapse image if a certain distance has been covered since the last image was captured

There is also a sample viewer application which can unwrap an MJPG stream from the Pi or cloud streaming server.