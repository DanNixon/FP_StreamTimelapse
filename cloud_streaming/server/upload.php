<?php

$key = "FPST_CLOUD_DEV";

if($_POST['key'] != $key) {
    die("AUTH_FAIL");
}

if (move_uploaded_file($_FILES['frame']['tmp_name'], "/var/www/frame/i.jpg")) {
    echo "FRAME_UPLOAD";
    chmod("/var/www/frame/i.jpg", 0777);
} else {
    die("FRAME_FAIL");
}

?>
