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

function send_tl_request(tl_path) {
	var url = "http://37.139.30.37/generate_video.php?";
	url += ("tl=" + tl_path);
	url += ("&l_lim=" + document.getElementById("l-lim").value);
	url += ("&u_lim=" + document.getElementById("u-lim").value);
	console.log(url);
	window.location.replace(url);
}

function reg_events() {
	var original_gen = document.getElementById("gen_orig");
	if(original_gen.addEventListener) {
		original_gen.addEventListener("click", function() {
			send_tl_request("/i%d.jpg");
		});
	} else {
		original_gen.attachEvent("click", function() {
			send_tl_request("/i%d.jpg");
		});
	}
	
	var equi_gen = document.getElementById("gen_equi");
	if(equi_gen.addEventListener) {
		equi_gen.addEventListener("click", function() {
			send_tl_request("/i%d_e.jpg");
		});
	} else {
		equi_gen.attachEvent("click", function() {
			send_tl_request("/i%d_e.jpg");
		});
	}
}

self.setInterval(function(){check_file()}, 1000);

if(window.addEventListener) {
	window.addEventListener("load", reg_events, false);
} else if(window.attachEvent) {
	window.attachEvent("onload", reg_events);
} else {
	document.addEventListener("load", reg_events, false);
}
</script>
</head>
<body>
<p>
	Lower frame limit: <input type="text" size="3" id="l-lim" value="<?php if(isset($_GET['ll'])) { echo $_GET['ll']; } else { echo 0; }?>">
	Upper frame limit: <input type="text" size="3" id="u-lim" value="<?php if(isset($_GET['ul'])) { echo $_GET['ul']; } else { if(isset($_GET['ll'])) { echo intval($_GET['ll']) + 200; } else { echo 200; }}?>">
</p>
<p id="gen_orig">Generate Orignal Timelapse</p>
<p id="gen_equi">Generate Equi Timelapse</p>
<?php
if(isset($_GET['tl'])) {
	$tl_filename = sprintf("tl_%d.mp4", time());
	exec('/var/www/tl_gen.sh '.$_GET['l_lim'].' '.$_GET['u_lim'].' /var/www/tl_frames'.$_GET['tl'].' "/var/www/tl_exports/'.$tl_filename.'" > /dev/null &');
	echo("<p id='filename'>".$tl_filename."</p>");
}
?>
<br />
<p><a href="/tl_exports">View Exported Timelapse  Videos</a></p>
</body>
</html>
