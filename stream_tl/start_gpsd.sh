#!/bin/bash

PORT=/dev/ttyUSB0

sudo gpsd $PORT -F /var/run/gpsd.sock
