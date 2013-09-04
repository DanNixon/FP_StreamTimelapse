Cloud Streaming Example
=======================

A sample/test showing the use of an intermediate webserver between the Pi and the client, reducing load on the Pi.

Also included is a sample web page which allows viewing an unwrapped stream and the current GPS location using Google Maps.

This also allows uploading of the timelapse frames to a server and having the server create the timelaspe video, this simplifies the process of exporting content form the Pi and allows the PI to capture for a longer period of time (assuming a stable internet connection is available) as frames that have been uploaded will be automatically deleted.

Requirements
------------

In addition to the general requirements:

-	A working FP_StreamTimelaspe setup
-	Pi
	-	[poster](https://pypi.python.org/pypi/poster/)
	-	[pyinotify](https://github.com/seb-m/pyinotify)
-	Server
	-	[mjpg-streamer](http://sourceforge.net/projects/mjpg-streamer/) ```.deb``` package, [this](http://liangsun.org/posts/resolve-error-version-number-does-not-start-with-digit) shows how to deal with the dpkg error
	-	```libjpeg62```
	-	```php5```
	-	```apache2```
	-	```ffmpeg```
	-	```libavcodec-extra-53``` (on Ubuntu server 12.04)

Usage
-----

Browse to the root of the server to see the sample streaming page.

To export a timelapse browse to ```timelapse.php``` you can then select to export either a timelaspe using the original frames or equirectangular images, once conversion has completed you will be able to download the MP4 video.

Browse to ```timelapse_viewer.html``` to view the saved timelapse frames.

Setup
-----

Both ```upload.php``` and ```imageWatcher.py``` have a key variable which must be set to the same value on both scripts (a very simplistic authentication for this demo).

See setup details in each folder for setup specifics for Pi and server.