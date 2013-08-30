#!/bin/bash

cd /var/www

dir_found=0
i=0
while [ $dir_found == 0 ]
do
	if [ -d "f"$i ]; then
		(( i++ ))
	else
		dir_found=1
		mkdir f$i
	fi
done

mv $1 ./f$i/original.jpg

if [ -f ./f$i/original.jpg ]; then
	./equi_gen ./f$i/original.jpg ./f$i/equi.jpg 400

	##Uncomment to stream equi
	cp ./f$i/equi.jpg ./frame/i.jpg

	##Uncomment to stream original
	#cp ./f$i/original.jpg ./frame/i.jpg

	##Do other things with original and equi images here if needed
	##e.g. numbering for archive, etc
fi

rm -r ./f$i
