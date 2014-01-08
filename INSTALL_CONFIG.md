Install/Config
==============

Requirements
------------

1.	Install/obtain/configure the following:
	-	[CMake](http://www.cmake.org): ```cmake```
	-	[OpenCV](http://opencv.willowgarage.com): ```libopencv-core2.3```, ```libopencv-core-dev```, ```libopencv-dev```, ```libopencv-imgproc2.3``` (used for equi unwrap)
	-	[GPSd](http://www.catb.org/gpsd): ```gpsd```, ```libgps-dev```, ```libgps20```, ```python-gps``` (for communication between software and GPS hardware)
	-	[Exiv2](http://www.exiv2.org/index.html): ```libexiv2-12```, ```libexiv2-dev``` (C++ library for EXIF data read/write)
	-	[mjpg-streamer](http://sourceforge.net/projects/mjpg-streamer/) (used to create MJPG stream, included in this repo pre-configured)
	-	```screen```
	-	Camera module adapted for variable focus (glue on lens screw removed)
	-	"Large" SD card (timelapse capturing every minute should run for 7 days happily on a 32GB card)
	-	Suitable GPS module compatible with gpsd and with fix indicator output (e.g. [Adafruit Ultimate GPS](http://www.adafruit.com/products/746))
	-	[GPS antenna](http://www.adafruit.com/products/960) and any [adapters](http://www.adafruit.com/products/851) needed
	-	GPIO cable (either the [standalone cable](http://uk.farnell.com/adafruit-industries/862/gpio-ribbon-cable-26-pin-raspberry/dp/2215033) or [breadboard breakout](http://uk.farnell.com/adafruit-industries/914/pi-cobbler-breakout-kit-raspberry/dp/2215039) type)
	-	Breadboard/prototyping board (such as [this](http://www.ebay.co.uk/itm/Mini-Prototype-Breadboard-for-Arduino-400-Tie-Point-Solderless-Modular-Board-/140711857203) one)
	-	Breadboard patch cables (such as [these](http://www.ebay.co.uk/itm/75-Jumper-Cables-for-Solderless-Breadboard-Arduino-/171108244207))
	-	Pi enclosure ([this](http://uk.farnell.com/multicomp/mc-rp001-clr/enclosure-raspberry-pi-clear/dp/2113799) one is good as it has keyhole mounting brackets on the rear)
	-	Soldering iron & solder (the Adafruit parts may need basic assembly)

Software Installation
---------------------

2.  Clone repo onto Pi SD card
3.  Set path to stream_tl in ```start_timelapse.sh```, ```stop_timelaspe.sh``` and ```recovery.sh```
4.  Optionally use ```crontab -e -u pi``` to start capture on boot by adding the following line to the crontab: ```@reboot /home/pi/stream_tl/start_timelapse.sh 10 >> /home/pi/stream_tl/cron_tl.log```
5. Set execute permissions for the user which will run the streaming app for each of the ```*.sh``` files and ```mjpg_streamer``` executable

Hardware Installation
---------------------

6.	Install camera module as directed [here](http://www.raspberrypi.org/camera), check it works using ```raspistill```.
7.	Disable hardware UART boot messages and tty:
	-	run ```sudo nano /boot/cmdline.txt``` and change
	-	**"dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait"** to
	-	**"dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait"**
8.	Disable hardware UART tty/getty:
	-	run ```sudo nano /etc/inittab``` and comment out the following line:
	-	**"T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100"**
9.	Connect the GPS module to the GPIO header:
	-	Connect GPIO pin 1 (3v3) to Vin/3v3 on GPS module
	-	Connect GPIO pin 6 (GND) to GPS GND
	-	Connect GPIO pin 8 (Txd) to GPS Rxd
	-	Connect GPIO pin 10 (Rxd) to GPS Txd
	-	Connect GPIO pin 16 (GPIO23) to GPS Fix Indicator (assuming active low, if active high change the "1" in this line of main.cpp to "0": ```if(fix_state == "1")```)
	-	Test GPS module using ```cgps -s```
10.	Configure GPSd: run ```sudo dpkg-reconfigure gpsd``` and set the following options:
	-	"Yes", start automatically
	-	"Yes", USB receivers are not used, but may as well
	-	"/dev/ttyAMA0", this is the name of the BCM UART which the GPS is attached to
	-	"-n -G", ```-n``` poll GPS before clients have connected (needed for some apps), ```-G``` allow GPSd to be accessed externally (good to allow other networked devices to get GPS data)
	-	Keep control socket as default
11.	Configure hardware controls:
	-	Solder a 2 pin .1" header onto the unpopulated P6 header on the Pi (between voltage regulator RG2 and HDMI port)
	-	Connect a PTM switch between pin 1 of P6 (the pin closest to the edge of the board, with a square copper pad) and any ground (pin 2 of P6 is ground), this is the start/boot button (see USAGE).
	-	Connect a PTM switch between pin 3 of the main GPIO header and ground, this is the stop button (see USAGE).

Configure timelapse
-------------------

12.  Configure delay between timelapse captures, this is set in [start_timelapse.sh](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/start_timelapse.sh), by default it is set to 30000ms.
13.	Configure timelapse image filenames, this can be set in [start_timelapse.sh](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/start_timelapse.sh), where ```%d``` is the frame number (important: if the filename does not have a %d the same file will be overwritten every capture)
14.	Configure GPS options:
	-	GPS can be enabled/disabled by editing the ```USE_GPS``` line of start_timelapse.sh
	-	To record GPS data only set min capture distance in start_timelapse.sh to 0
	-	Alternatively, set this to a distance (either miles or KM, see below) which has to have been covered in the timelapse delay if a new image is to be taken
	-	The unit of the above option and unit in which speed is recorded in GPS EXIF can be configured in gps.cpp, by changing the ```gps_speed_units``` option (note that the distance covered option is in KM in Knots is selected as the speed unit)

Configure streaming_timelapse
-----------------------------

15.	Configure timelapse camera options ( [main.cpp](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp) )
16.	Configure MJPG stream camera options ( [main.cpp](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp) )
17.	Configure misc timelapse options ( [main.cpp](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp) )
	-	min_tl_delay = having a timelapse period too low reduced stream consistency, a minimum helps prevent that
	-	tl_cap_run_in = time camera module is running for before capturing a timelapse image, improves consistency of exposure
18.	Configure equi. image generation ( [main.cpp](https://github.com/DanNixon/FP_StreamTimelapse/blob/master/stream_tl/streaming_timelapse/main.cpp) )
	-	Parameters same as general equi. generation

Optional Stuff
--------------

-	Disable Auto Recovery
	-	Auto recovery can get the Pi stuck in a reboot cycle if it fails (although this is very unlikely)
	- It can be disabled by commenting out marked lines of ```start_timelapse.sh```
- Custom web pages
	-	mjpg-streamer has a webserver, it's contents can be modified by changing the contents of the www folder inside mjpg-streamer
- Custom frame handling
	-	It is possible to used stream frame images with custom applications, in this case have your application start instead of mjpg-streamer in ```start_timelapse.sh``` and watch the ```stream_tl```	directory for the frame file to be modified

Notes
-----

You may wish to disable the camera LED when you have verified that the installation is working, this can be done by editing ```/boot/config.txt``` and adding the line ```disable_camera_led=1```, this helps to remove any visual defects on the image caused by light from the LED entering the bubble scope.

For both 5 and 6, options are passed to the ```raspistill``` command and control the image size and effects/filters.

- -n = no preview
- -h, -w = height and width
- Full raspicam/raspistill docs [here](https://github.com/raspberrypi/userland/blob/master/host_applications/linux/apps/raspicam/README.md)
- Better GPS setup instruction can be found [here](http://learn.adafruit.com/adafruit-ultimate-gps-on-the-raspberry-pi) (specific to Adafruit module)
