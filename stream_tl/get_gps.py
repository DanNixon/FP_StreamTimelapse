import sys, gps

#Prints GPS data to console
#Note: gpsd must be running and getting data from a sensor

session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)

while True:
	try:
		report = session.next()
		if report['class'] == 'TPV':
			print report['lat'],report['lon'],report['alt'],report['track'],report['speed'],report['time']
			exit()
			break
	except StopIteration:
		session = None
		print "0.0 0.0 0.0 0.0 0-0-0T0:0:0.0Z"
		break
