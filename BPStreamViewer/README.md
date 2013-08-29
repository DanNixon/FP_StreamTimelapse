BPStreamViewer
==============

Sample C++ app used to test and unwrap streams from the Pi or cloud streaming server.

Requirements
------------

-	[OpenCV](http://opencv.willowgarage.com) (tested with version 2.4)

Usage
-----

```./BPSphereViewer [IP/HOST]:[PORT] [function]```

-	```[PORT]``` will be 8080 unless it has been changed in the launch command for mjpg-streamer
-	Currently ```[function]``` can only be set to "unwrap", which creates an equi unwrap of the MJPG stream