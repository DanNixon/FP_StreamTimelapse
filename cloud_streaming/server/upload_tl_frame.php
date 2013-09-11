<?php

$key = "FPST_CLOUD_DEV";

if($_POST['key'] != $key) {
    die("AUTH_FAIL");
}

if (move_uploaded_file($_FILES['frame']['tmp_name'], "/var/www/tl_frames/".$_FILES['frame']['name'])) {
    echo "FRAME_UPLOAD";
} else {
    die("FRAME_FAIL");
}

?>
