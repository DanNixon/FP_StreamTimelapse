#!/bin/bash

ffmpeg -f image2 -r 10 -i $1 -vcodec libx264 -preset ultrafast -crf 25 "$2"

#-r FRAME RATE
#-crf QUALITY
