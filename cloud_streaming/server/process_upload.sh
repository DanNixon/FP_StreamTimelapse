#!/bin/bash

cd /var/www

./equi_gen $1 ./equi.jpg
mv $1 ./original.jpg

##Uncomment to stream equi
cp ./equi.jpg ./frame/i.jpg

##Uncomment to stream original
#cp ./original.jpg ./frame/i.jpg

##Do other things with original and equi images here if needed
##e.g. numbering for archive, etc
