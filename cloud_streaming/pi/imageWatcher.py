import pyinotify, shutil, os, thread, urllib2
from poster.encode import multipart_encode
from poster.streaminghttp import register_openers

use_gps = False
upload_count = 0
frame_filename = "/home/pi/stream_tl/s_frame.jpg"
service_key = "FPST_CLOUD_DEV"

def upload_handler():
	try:
		global upload_count
		local_file = "frame{0}.jpg".format(upload_count)
		upload_count += 1
		if upload_count % 50 == 0:
			print "imageWatcher: image_{0}".format(upload_count)
		shutil.copyfile(frame_filename, local_file)
		register_openers()
		post_data = {"frame" : open(local_file, "rb"), "key" : service_key}
		if use_gps:
			gps_file = open("./gps.temp")
			gps_string = gps_file.read()
			gps_file.close()
			gps_data = gps_string.split()
			try:
				post_data["lat"] = gps_data[0]
				post_data["lon"] = gps_data[1]
				post_data["alt"] = gps_data[2]
				post_data["track"] = gps_data[3]
				post_data["speed"] = gps_data[4]
				post_data["time"] = gps_data[5]
			except IndexError as gpse:
				print "imageWatcher: GPS Error", gpse
				pass
		datagen, headers = multipart_encode(post_data)
		request = urllib2.Request("http://37.139.30.37/upload.php", datagen, headers)
		result = urllib2.urlopen(request).read()
		if result != "FRAME_UPLOAD":
			print result
		os.remove(local_file)
	except urllib2.URLError as urle:
		print "imageWatcher: Network Error", urle
		pass
	except IOError as ioe:
		print "imageWatcher: IO Error", ioe
		pass

wm = pyinotify.WatchManager()
mask = pyinotify.IN_CLOSE_WRITE

class EventHandler(pyinotify.ProcessEvent):
	def process_IN_CLOSE_WRITE(self, event):
		if event.pathname == frame_filename:
			upload_handler()

handler = EventHandler()
notifier = pyinotify.Notifier(wm, handler)
wdd = wm.add_watch('/home/pi/stream_tl/', mask, rec=True)

notifier.loop()
