#!/bin/bash

##Usage: ./frame_subset.sh [start frame] [end frame] [original image pattern] [output folder]

mkdir ./"$4"
img_count=0

for ((f=$1; f <= $2 ; f++))
do
        cp `printf "$3" $f` ./"$4"/frame"$img_count".jpg
        img_count=$((img_count+1))
done
