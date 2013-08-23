GPSDEV
======

Stuff to do before merging with master.

ToDo
----

1.	Other EXIF tags into original (timestamp, speed, alt, track) - DONE!
2.	EXIFs into equi. image - DONE!
3.	Add timeout for getting GPS position
4.	Get Pi clock to sync with GPS - SORT OF DONE

Notes
-----

Did not include GPS timestamp, plan to get RasPi system clock to sync with GPS time on first lock, in which case EXIF capture timestamp will be sufficient (RasPi clock skew should be negligible).

GPS time is saved in ```EXIF.GPSInfo.GPSDateStamp``` and ```EXIF.GPSInfo.GPSTimeStamp```, use this if accurete time is needed, if Pi has an internet connection (or is at least on a network with an NTP server) then the system clock should be good enough.

Because a GPIO pin is used to read the fix indicator, the script must run with root permissions, meaning any images will belong to root (if you download images over SFTP then this isn't a problem).