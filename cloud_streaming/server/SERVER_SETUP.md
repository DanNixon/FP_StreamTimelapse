Server Setup
============

Setup streaming server:

1.	Copy ```upload.php``` to the web root of the Apache server (on Debian based linux usually ```/var/www```)
2.	Create a folder in web root named ```frame```
3.	Copy ```mjps.sh``` to the server, it is best placed inside ```/var/www```
4.	To configure mjpg-streamer to start on boot run ```sudo crontab -e -u www-data``` and add the following line to the crontab: ```@reboot /var/www/mjps.sh &```