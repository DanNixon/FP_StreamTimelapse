#!/bin/bash

echo python tlUpload.py ./tl_images/equi/"$1"_e.jpg
UPLOAD_STATE=$?

if [ "$UPLOAD_STATE" == "0" ];
then
	rm ./tl_images/equi/"$1"_e.jpg
fi

python tlUpload.py ./tl_images/original/"$1".jpg
UPLOAD_STATE=$?

if [ "$UPLOAD_STATE" == "0" ];
then
	rm ./tl_images/original/"$1".jpg
fi
