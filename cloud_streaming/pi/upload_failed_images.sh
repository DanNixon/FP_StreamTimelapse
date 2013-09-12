#!/bin/bash

echo "Stopping timelapse"
./stop_timelapse.sh

echo "Uploading originals"
for file in tl_images/original/*.jpg ; do
	echo "Uploading $file"
	python tlUpload.py "$file"
	UPLOAD_STATE=$?
	if [ "$UPLOAD_STATE" == "0" ];
	then
		sudo rm "$file"
	fi
done

echo "Uploading equis"
for file in tl_images/equi/*.jpg ; do
	echo "Uploading $file"
	python tlUpload.py "$file"
	UPLOAD_STATE=$?
	if [ "$UPLOAD_STATE" == "0" ];
	then
		sudo rm "$file"
	fi
done

echo "Uploads done, timelapse can now be restarted with ./start_timelapse.sh"
