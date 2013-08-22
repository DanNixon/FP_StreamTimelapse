FP_StreamTimelapse
==================

360 degree timelapse recorder for BubbleScope with MJPEG streaming to run on the RasPi.

Requirements
------------

1.  OpenCV (tested with 2.4)
2.  RaspiStill
3.  cmake
4.	Camera module adapted for variable focus (glue on lens screw removed)
5.	"Large" SD card (timelapse capturing every minute should run for 7 days happily on a 32GB card)
6. [mjpg-streamer](http://sourceforge.net/projects/mjpg-streamer/)
7. gpsd
8. Suitable GPS module compatible with gpsd and with fix indicator output (e.g. [Adafruit Ultimate GPS](http://www.adafruit.com/products/746))
9. [Exiv2](http://www.exiv2.org/index.html) (C++ library for EXIF data read/write)