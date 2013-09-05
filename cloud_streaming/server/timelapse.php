<html>
<head>
<title>Timelapse Generation</title>
<script>
function file_exists(url)
{
    var http = new XMLHttpRequest();
    http.open('HEAD', url, false);
    http.send();
    return http.status != 404;
}

function check_file() {
	var filename = document.getElementById("filename").innerText;
	var tl_url = "./tl_exports/" + filename;
	if(file_exists(tl_url)) {
		document.getElementById("filename").innerHTML = "<a href='" + tl_url + "'>Download Timelapse</a>";
	}
}

self.setInterval(function(){check_file()}, 1000);
</script>
</head>
<body>
<p><a href="timelapse.php?tl=/i%d.jpg">Generate Orignal Timelapse</a></p>
<p><a href="timelapse.php?tl=/i%d_e.jpg">Generate Equi Timelapse</a></p>
<?php
if(isset($_GET['tl'])) {
	$tl_filename = sprintf("tl_%d.mp4", time());
	exec('/var/www/tl_gen.sh /var/www/tl_frames'.$_GET['tl'].' "/var/www/tl_exports/'.$tl_filename.'" > /dev/null &');
	echo("<p id='filename'>".$tl_filename."</p>");
}
?>
<br />
<p><a href="/tl_frames">View All Timelapse Frames</a></p>
<p><a href="/tl_exports">View All Timelapse Videos</a></p>
</body>
</html>
