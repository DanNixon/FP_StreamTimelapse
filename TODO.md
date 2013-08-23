GPSDEV
======

Stuff to do before merging with master.

ToDo
----

1.	Other EXIF tags into original (timestamp, speed, alt, track) - DONE!
2.	EXIFs into equi. image - DONE!
3.	Add timeout for getting GPS position
4.	Get Pi clock to sync with GPS

Notes
-----

Did not include GPS timestamp, plan to get RasPi system clock to sync with GPS time on first lock, in which case EXIF capture timestamp will be sufficient (RasPi clock skew should be negligible).