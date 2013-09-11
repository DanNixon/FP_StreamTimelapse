import sys, shutil, os, thread, urllib2
from poster.encode import multipart_encode
from poster.streaminghttp import register_openers

service_key = "FPST_CLOUD_DEV"

try:
	frame_filename = sys.argv[1]
	register_openers()
	post_data = {"frame" : open(frame_filename, "rb"), "key" : service_key}
	datagen, headers = multipart_encode(post_data)
	request = urllib2.Request("http://[HOST_IP]/upload_tl_frame.php", datagen, headers)
	result = urllib2.urlopen(request).read()
	if result != "FRAME_UPLOAD":
		print result
		sys.exit(1)
	sys.exit(0)
except urllib2.URLError:
	print "tlUplaod: Network Error"
	sys.exit(1)
except IOError:
	print "tlUplaod: IO Error"
	sys.exit(1)
except IndexError:
	print "tlUpload: Filename Error"
	sys.exit(1)
