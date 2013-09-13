#!/bin/bash

##Usage tl_gen.sh [lower limit] [upper limit] [origin file pattern] [output filename] [output width] [frame rate] [hist equal]

##-r FRAME RATE
##-crf QUALITY

cd /var/www

mkdir "$4"_frames/
img_count=0

for ((f=$1; f <= $2 ; f++))
do
        cp `printf "$3" $f` "$4"_frames/frame"$img_count".jpg
        img_count=$((img_count+1))
done

if [ "$7" == "1" ]; then
	for f in "$4"_frames/*
	do
		convert $f -equalize "$f"
	done
fi

ffmpeg -f image2 -r $6 -i "$4"_frames/frame%d.jpg -vcodec libx264 -preset ultrafast -crf 25 -vf scale=$5:-1 "$4"_temp.mp4

mv "$4"_temp.mp4 "$4"

rm -r "$4"_frames
