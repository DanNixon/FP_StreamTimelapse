import sys, gps

#Prints GPS lat/lon to console, optionally saves more location data to file
#Note: gpsd must be running and getting data from a sensor
#USAGE: python get_gps.py [output_file]

# Listen on port 2947 (gpsd) of localhost
session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)

if len(sys.argv) > 1:
	out_file = open(sys.argv[1], 'w')

while True:
	try:
		report = session.next()
		if report['class'] == 'TPV':
			if len(sys.argv) > 1:
				for key in {"lat", "lon", "alt", "track", "speed", "climb", "time"}:
					out_file.write(key)
					out_file.write(":")
					out_file.write(str(report[key]))
					out_file.write("\n")
				out_file.close()
			print report['lat'],report['lon'],report['alt'],report['track'],report['speed']
			break
	except StopIteration:
		session = None
		print "0.0 0.0 0.0 0.0"
