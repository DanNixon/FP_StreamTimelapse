<?php
if(isset($_GET['frame'])) {
	get_exif($_GET['frame']);
} else {
	die("NO_FRAME");
}

function get_exif($frame_url) {
	$exif = exif_read_data($frame_url, 0, true);
	$gps = $exif['GPS'];

	$time_string = $gps['GPSTimeStamp'];
	$date_string = $gps['GPSDateStamp'];
	
	if(empty($time_string) or empty($date_string)) {
		$dt_string = $exif['EXIF']['DateTimeOriginal'];
		$td_data = explode(":", $dt_string);
		
		//TODO: This will need changed when https://github.com/raspberrypi/userland/pull/85 is merged
		$time = sprintf("%'02d:%'02d:%'02d", $td_data[3], $td_data[4], $td_data[5]);
		$date = sprintf("%'02d/%'02d/%'04d", $td_data[2], $td_data[1], $td_data[0]);
	} else {
		$time = timestamp_to_string($time_string);
		$date = datestamp_to_string($date_string);
	}

	$lat = dms_to_decimal($gps['GPSLatitude'], $gps['GPSLatitudeRef']);
	$lon = dms_to_decimal($gps['GPSLongitude'], $gps['GPSLongitudeRef']);
	$track = rational_to_decimal($gps['GPSTrack']);
	$alt = rational_to_decimal($gps['GPSAltitude']);
	$speed = rational_to_decimal($gps['GPSSpeed']);

	$json_string = sprintf('{"lat":%F,"lon":%F,"alt":%F,"track":%F,"speed":%F,"time":"%s","date":"%s"}', $lat, $lon, $alt, $track, $speed, $time, $date);
	echo($json_string);
}

function rational_to_decimal($rational_str) {
	$rational_comps = split('/', $rational_str);
	$result = $rational_comps[0] / $rational_comps[1];
	return $result;
}

function dms_to_decimal($dms, $ref) {
	$deg = rational_to_decimal($dms[0]);
	$min = rational_to_decimal($dms[1]);
	$sec = rational_to_decimal($dms[2]);
	$result = $deg + ($min / 60) + ($sec / 3600);
	$neg_refs = array("S", "W");
	if(in_array($ref, $neg_refs)) {
		return -$result;
	}
	return $result;
}

function timestamp_to_string($timestamp) {
	$hour = rational_to_decimal($timestamp[0]);
	$min = rational_to_decimal($timestamp[1]);
	$sec = rational_to_decimal($timestamp[2]);
	$result = sprintf("%'02d:%'02d:%'02d", $hour, $min, $sec);
	return $result;
}

function datestamp_to_string($datestamp) {
	$year = substr($datestamp, 0, 4);
	$month = substr($datestamp, 4, 2);
	$day = substr($datestamp, 6, 2);
	$result = sprintf("%'02d/%'02d/%'04d", $day, $month, $year);
	return $result;
}
?>
