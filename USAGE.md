Usage
=====

Note: assuming SSH access opposed to local terminal.

If capture is configured to start on boot type ```sudo reboot``` to reboot the Pi in order to start capture, otherwise execute ```start_timelapse.sh``` to start capture.

If after around 3 the light on the RasPI camera module is lit then capture has successfully started. In both cases capture can be stopped by executing ```stop_timelapse.sh```.

If the Pi fails to start capture then it will attempt to perform auto recovery, in this process it will execute ```recovery.sh```, cleaning the directory of images and logfiles (by moving them to an archive folder) and rebuilding the capture application.

Troubleshooting
---------------

Use either ```ps aux | grep stream``` or ```screen -ls``` to check status of capture, ~5 processes should be shown with ps and two screens should be shown with screen.

From the root folder (stream_tl) use ```tail -f screenlog.0``` to monitor capture in real time.