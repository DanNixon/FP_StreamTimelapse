<?php

$key = "FPST_CLOUD_DEV";

if($_POST['key'] != $key) {
    die("AUTH_FAIL");
}

if (move_uploaded_file($_FILES['frame']['tmp_name'], "/var/www/d.jpg")) {
    echo "FRAME_UPLOAD";
    exec("/var/www/process_upload.sh /var/www/d.jpg");
    
    $gps_file = fopen("/var/www/gps.temp", 'w');
    $gps_data = sprintf('{"lat":%s, "lon":%s, "track":%s, "speed":%s, "alt":%s, "time":"%s"}', $_POST['lat'], $_POST['lon'], $_POST['track'], $_POST['speed'], $_POST['alt'], $_POST['time']);
    fwrite($gps_file, $gps_data);
    fclose($gps_file);
    
    $gps_log_filename = "/var/www/gps.log";
    $gps_log_file = fopen($gps_log_filename, 'a+');
    $log_contents = fread($gps_log_file, filesize($gps_log_filename));
    $gps_log_data = str_getcsv($log_contents);
    $last_timestamp = $gps_log_data[sizeof($gps_log_data) - 1];
    if($last_timestamp != $_POST['time']) {
		$gps_log_data = sprintf("%s,%s,%s,%s,%s,%s\n", $_POST['lat'], $_POST['lon'], $_POST['track'], $_POST['speed'], $_POST['alt'], $_POST['time']);
		fwrite($gps_log_file, $gps_log_data);
	}
    fclose($gps_log_file);
} else {
    die("FRAME_FAIL");
}

?>
