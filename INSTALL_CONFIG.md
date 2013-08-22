Install/Config
==============

Software Installation
---------------------

1.  Clone repo onto Pi SD card
2.  Set path to stream_tl in ```start_timelapse.sh```, ```stop_timelaspe.sh``` and ```recovery.sh```
3.  Optionally use ```crontab -e -u pi``` to start capture on boot using command and line from ```crontab.txt```

Hardware Installation
---------------------

4.	Install camera module as directed [here](http://www.raspberrypi.org/camera), check it works using ```raspistill```.
5.	Disable hardware UART boot messages and tty:
	-	run ```sudo nano /boot/cmdline.txt``` and change
	- "dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait" to

	- **"dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait"**
6.	Disable hardware UART tty/getty:
	-	run ```sudo nano /etc/inittab``` and comment out the following line:
	-	**"T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100"**
7.	Connect the GPS module to the GPIO header:
	-	Connect GPIO pin 1 (3v3) to Vin/3v3 on GPS module (note GPS module must be 5v logic compliant, or be able to be powered from 3.3v)
	-	Connect GPIO pin 6 (GND) to GPS GND
	-	Connect GPIO pin 8 (Txd) to GPS Rxd
	-	Connect GPIO pin 10 (Rxd) to GPS Txd
	-	Connect GPIO pin 16 (GPIO23) to GPS Fix Indicator (assuming active low, if active high change the "1" in this line of main.cpp to "0": ```if(fix_state == "1")```)
	-	Test GPS module using ```cgps -s```

Configure timelapse
-------------------

8.  Configure delay between timelapse captures, this is set in [start_timelapse.sh](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/start_timelapse.sh#L22), by default it is set to 5000ms.
9.	Configure timelapse image filenames, this can be set in [start_timelapse.sh](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/start_timelapse.sh#L22), where ```%d``` is the frame number (this is very important, if the filename does not have a %d the same file will be overwritten every capture)

Configure streaming_timelapse
-----------------------------

10.	Configure timelapse camera options ( [main.cpp L17](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp#L17) )
11.	Configure MJPG stream camera options ( [main.cpp L18](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp#L18) )
12.	Configure misc timelapse options ( [main.cpp L19-20](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp#L19) )
	-	min_tl_delay = having a timelapse period too low reduced stream consistency, a minimum helps prevent that
	-	tl_cap_run_in = time camera module is running for before capturing a timelapse image, improves consistency of exposure
13.	Configure equi. image generation ( [main.cpp L42](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp#L42) )
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
- Better GPS setup instruction can be found [here](http://learn.adafruit.com/adafruit-ultimate-gps-on-the-raspberry-pi) (specific to Adafruit module)