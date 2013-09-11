#!/bin/bash

##Usage tl_gen.sh [lower limit] [upper limit] [origin file pattern] [output filename]

##-r FRAME RATE
##-crf QUALITY

echo $1
echo $2
echo $3
echo $4

cd /var/www

mkdir "$4"_frames/
img_count=0

for ((f=$1; f <= $2 ; f++))
do
        cp `printf "$3" $f` "$4"_frames/frame"$img_count".jpg
        img_count=$((img_count+1))
done

ffmpeg -f image2 -r 10 -i "$4"_frames/frame%d.jpg -vcodec libx264 -preset ultrafast -crf 25 "$4"_temp.mp4

mv "$4"_temp.mp4 "$4"

rm -r "$4"_frames
