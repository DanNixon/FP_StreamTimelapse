<?php

$key = "FPST_CLOUD_DEV";

if($_POST['key'] != $key) {
    die("AUTH_FAIL");
}

if (move_uploaded_file($_FILES['frame']['tmp_name'], "/var/www/d.jpg")) {
    echo "FRAME_UPLOAD";
    exec("/var/www/process_upload.sh /var/www/d.jpg");
    $gps_file = fopen("/var/www/gps.txt", 'w');
    $gps_data = sprintf('{"lat":%s, "lon":%s, "track":%s, "speed":%s, "alt":%s}', $_POST['lat'], $_POST['lon'], $_POST['track'], $_POST['speed'], $_POST['alt']);
    fwrite($gps_file, $gps_data);
    fclose($gps_file);
} else {
    die("FRAME_FAIL");
}

?>
