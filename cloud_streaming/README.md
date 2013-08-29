Cloud Streaming Example
=======================

A sample/test showing the use of an intermediate webserver between the Pi and the client, reducing load on the Pi.

Requirements
------------

In addition to the general requirements:

-	A working FP_StreamTimelaspe setup
-	Pi
	-	[poster](https://pypi.python.org/pypi/poster/)
	-	[pyinotify](https://github.com/seb-m/pyinotify)
-	Server
	-	[mjpg-streamer](http://sourceforge.net/projects/mjpg-streamer/) ```.deb``` package, [this](http://liangsun.org/posts/resolve-error-version-number-does-not-start-with-digit) will be useful
	-	```libjpeg62```
	-	```php5```
	-	```apache2```

Setup
-----

Both ```upload.php``` and ```imageWatcher.py``` have a key variable which must be set to the same value on both scripts (a very simplistic authentication for this demo).

See setup details in each folder for setup specifics for Pi and server.