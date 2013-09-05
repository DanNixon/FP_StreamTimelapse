Server Setup
============

Setup streaming server:

1.	Copy ```upload.php``` to the web root of the Apache server (on Debian based linux usually ```/var/www```)
2.	Create a folder in web root named ```frame```
3.	Copy ```mjps.sh``` to the server, it is best placed inside ```/var/www```
4.	To configure mjpg-streamer to start on boot run ```sudo crontab -e -u www-data``` and add the following line to the crontab: ```@reboot /var/www/mjps.sh &```
5.	Configure the image resolution that it will receive from the Pi, this is used to prevent attempting an unwrap of an incomplete image. This can be set on the line beginning with ```./equi_gen``` in ```process_upload.sh```, it is the last integer parameter (default 400px, note that ```equi_gen``` expects a square image)

Note
----

-	```mjps.sh```, ```process_upload.sh```, ```index.html``` and ```upload.php``` are for MJPG streaming.
-	```timelapse.php``` and ```tl_gen.sh``` are for the timelapse exporting page.
-	```timelapse_viewer.html``` and ```get_image_gps.php``` are for the timelapse frame viewer.
-	```upload_tl_frame.php``` is used by the timelapse export page and frame viewer.
-	```gps_log_viewer.html``` is used to view data from ```gps.log``` (created by ```upload.php```)

It may be necessary to change the ```upload_max_filesize``` and ```post_max_size``` parameters in ```php.ini``` to upload original images.

You may see lots of this error ```libdc1394 error: Failed to initialize libdc1394``` in Apache ```error.log``` (particularly on virtual server instances), it is to do with camera hardware communication and comes from OpenCV, you can ignore it.
