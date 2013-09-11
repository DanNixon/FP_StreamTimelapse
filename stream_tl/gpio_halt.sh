#!/bin/bash

##GPIO to poll, use an I2C pin as they have internal pull up resistors
GPIO_PIN="2"

##0 for Low, 1 for High
HALT_ON="0"

echo $GPIO_PIN > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio$GPIO_PIN/direction

while :
do
	GPIO_VALUE=`cat /sys/class/gpio/gpio$GPIO_PIN/value`
	if [ $GPIO_VALUE == $HALT_ON ];
	then
		echo "gpio_halt.sh: Got halt conditions"
		cd /home/pi/stream_tl
		./stop_timelapse.sh
		sleep 30 ##This sleep time should be at least that of the timelapse period
		sudo shutdown -h now
		break
	fi
	sleep 1
done

echo $GPIO_PIN > /sys/class/gpio/unexport
