Cloud Streaming Example
=======================

A sample/test showing the use of an intermediate webserver between the Pi and the client, reducing load on the Pi.

Also included is a sample web page which allows viewing an unwrapped stream and the current GPS location using Google Maps.

This also allows uploading of the timelapse frames to a server and having the server create the timelaspe video, this simplifies the process of exporting content form the Pi and allows the PI to capture for a longer period of time (assuming a stable internet connection is available) as frames that have been uploaded will be automatically deleted.

A CSV log of the GPS data is also stored in ```gps.log``` which is added to on every upload of a streaming frame and can be viewed on a Google Map.

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
	-	```convert```, part of [ImageMagick](http://www.imagemagick.org/)
	-	```redist```, one of Fred's ImageMagick [scripts](http://www.fmwconcepts.com/imagemagick/redist/)
	-	```bc```, required by ```redist``` (not installed as standard on Ubuntu server I found)

Usage
-----

The root of the server contains a menu showing the various demos.

The "View Live Stream" page shows the current equi stream from the Pi as well as current GPS position data.

"Web Timelapse Viewer" allows viewing timelapse frames stored on the server and GPS position data associated with them.

"Export Timelapse Videos" allows you to export either a timelaspe using the original frames or equirectangular images, once conversion has completed you will be able to download the MP4 video.

When exporting timelapse videos the "Use histogram equalization" may be used to reduce the effects of bad exposure or colour balance by the Pi camera, however this will slightly reduce the overall image quality of the frames which are most affected.

"View GPS Log on Map" is used to view stored GPS data displayed using a Polyline which shows further data (altitude, heading, speed, time) when the mouse is moved over the line.

On the Pi, the ```upload_failed_images.sh``` script can be used to upload any images that have failed to automatically upload (due to network errors for example), if a timelapse video generation seems to have failed (video is shorter then expected) there is a good chance that this will fix the problem.

Setup
-----

Both ```upload.php``` and ```imageWatcher.py``` have a key variable which must be set to the same value on both scripts (a very simplistic authentication for this demo).

See setup details in each folder for setup specifics for Pi and server.