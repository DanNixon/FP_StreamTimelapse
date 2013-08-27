import sys, gps

#Prints GPS data to console
#Note: gpsd must be running and getting data from a sensor

session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)

while True:
	try:
		report = session.next()
		if report['class'] == 'TPV':
			res_str = str(report['lat']) + " "
			res_str += str(report['lon']) + " "
			res_str += str(report['alt']) + " "
			res_str += str(report['track']) + " "
			res_str += str(report['speed']) + " "
			res_str += report['time']
			data_file = open(sys.argv[1], "w")
			data_file.write(res_str)
			data_file.close()
	except StopIteration:
		session = None
		break
	except KeyError:
		pass
