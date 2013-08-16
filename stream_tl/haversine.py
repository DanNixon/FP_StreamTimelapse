import sys
from math import *

#Using Haversine algorithm, calculate distance between two lat/lon positions
#USAGE: python haversine.py lat1 long1 lat2 long2

def haversine(pos1, pos2):
	lat1 = float(pos1['lat'])
	long1 = float(pos1['long'])
	lat2 = float(pos2['lat'])
	long2 = float(pos2['long'])

	degree_to_rad = float(pi / 180.0)

	d_lat = (lat2 - lat1) * degree_to_rad
	d_long = (long2 - long1) * degree_to_rad

	a = pow(sin(d_lat / 2), 2) + cos(lat1 * degree_to_rad) * cos(lat2 * degree_to_rad) * pow(sin(d_long / 2), 2)
	c = 2 * atan2(sqrt(a), sqrt(1 - a))
	km = 6367 * c
	mi = 3956 * c

	return {"km":km, "miles":mi}

p1 = {"lat":sys.argv[1], "long":sys.argv[2]}
p2 = {"lat":sys.argv[3], "long":sys.argv[4]}
hs = haversine(p1, p2)
print hs['km'] #Select miles/km
