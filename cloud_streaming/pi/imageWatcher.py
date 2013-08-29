import pyinotify, shutil, os, thread, urllib2
from poster.encode import multipart_encode
from poster.streaminghttp import register_openers

upload_count = 0
frame_filename = "/home/pi/stream_tl/s_frame.jpg"
service_key = "FPST_CLOUD_DEV"

def upload_handler():
	global upload_count
	local_file = "frame{0}.jpg".format(upload_count)
	upload_count += 1
	shutil.copyfile(frame_filename, local_file)
	register_openers()
	datagen, headers = multipart_encode({"frame" : open(local_file, "rb"), "key" : service_key})
	request = urllib2.Request("http://[SERVER_IP]/upload.php", datagen, headers)
	print urllib2.urlopen(request).read()
	os.remove(local_file)

wm = pyinotify.WatchManager()
mask = pyinotify.IN_CLOSE_WRITE

class EventHandler(pyinotify.ProcessEvent):
	def process_IN_CLOSE_WRITE(self, event):
		if event.pathname == frame_filename:
			print "New image"
			upload_handler()


handler = EventHandler()
notifier = pyinotify.Notifier(wm, handler)
wdd = wm.add_watch('/home/pi/stream_tl/', mask, rec=True)

notifier.loop()