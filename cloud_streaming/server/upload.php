<?php

$key = "FPST_CLOUD_DEV";

if($_POST['key'] != $key) {
    die("AUTH_FAIL");
}

if (move_uploaded_file($_FILES['frame']['tmp_name'], "/var/www/d.jpg")) {
    echo "FRAME_UPLOAD";
    exec("/var/www/equi_gen /var/www/d.jpg /var/www/frame/i.jpg");
    exec("rm /var/www/d.jpg");
    chmod("/var/www/frame/i.jpg", 0777);
} else {
    die("FRAME_FAIL");
}

?>
