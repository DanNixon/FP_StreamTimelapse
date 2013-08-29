#Usage: python tl_gen.py [resolution] [duration_sec] [delay_sec]

import sys

file_sizes = {	2000:128.4,
		1500:86.3,
		1000:49.5}
file_sizes_for_i = 50

resolution = int(sys.argv[1])
tl_duration = int(sys.argv[2])
tl_period = int(sys.argv[3])

tl_images = tl_duration / tl_period
print "Images: ",tl_images
file_size_coeff = float(tl_images) / float(file_sizes_for_i)
image_size = file_size_coeff * file_sizes[resolution]
print "Image size: ", image_size

total_size = 4000 + image_size
print "SD card min.: ", total_size
