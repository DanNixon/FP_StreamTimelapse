<?php

if(isset($_GET['frame'])) {
	get_exif($_GET['frame']);
} else {
	die("NO_FRAME");
}

function get_exif($frame_url) {
	$exif = exif_read_data($frame_url, 0, true);
	$gps = $exif['GPS'];

	$lat = dms_to_decimal($gps['GPSLatitude'], $gps['GPSLatitudeRef']);
	$lon = dms_to_decimal($gps['GPSLongitude'], $gps['GPSLongitudeRef']);
	$track = rational_to_decimal($gps['GPSTrack']);
	$alt = rational_to_decimal($gps['GPSAltitude']);
	$speed = rational_to_decimal($gps['GPSSpeed']);

	$json_string = sprintf('{"lat":%F,"lon":%F,"alt":%F,"track":%F,"speed":%F}', $lat, $lon, $alt, $track, $speed);
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
?>
