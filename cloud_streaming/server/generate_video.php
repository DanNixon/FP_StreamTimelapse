<html>
<head>
<title>Timelapse Generation</title>
<script>
var host = "http://fp.dan-nixon.com/";
	
function file_exists(url)
{
    var http = new XMLHttpRequest();
    http.open('HEAD', url, false);
    http.send();
    return http.status != 404;
}

function check_file() {
	var filename = document.getElementById("filename").innerText;
	var tl_url = "tl_exports/" + filename;
	if(file_exists(host + tl_url)) {
		document.getElementById("filename").innerHTML = "<a href='" + tl_url + "'>Download Timelapse</a>";
	}
}

function send_tl_request(tl_path) {
	var url = host + "generate_video.php?";
	url += ("tl=" + tl_path);
	url += ("&ll=" + document.getElementById("l-lim").value);
	url += ("&ul=" + document.getElementById("u-lim").value);
	url += ("&width=" + document.getElementById("px-w").value);
	url += ("&filename=" + document.getElementById("filename-s").value);
	url += ("&frate=" + document.getElementById("framerate-s").value);
	if(document.getElementById("heq").checked) {
		url += ("&histeq=1");
	} else {
		url += ("&histeq=0");
	}
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
	Lower frame limit: <input type="text" size="3" id="l-lim" value="<?php if(isset($_GET['ll'])) { echo $_GET['ll']; } else { echo 0; }?>"><br />
	Upper frame limit: <input type="text" size="3" id="u-lim" value="<?php if(isset($_GET['ul'])) { echo $_GET['ul']; } else { if(isset($_GET['ll'])) { echo intval($_GET['ll']) + 200; } else { echo 200; }}?>"><br />
	Output video width: <input type="text" size="3" id="px-w" value="<?php if(isset($_GET['width'])) { echo $_GET['width']; } else { echo 2000; }?>"><br />
	Frame rate: <input type="text" size="3" id="framerate-s" value="<?php if(isset($_GET['frate'])) { echo $_GET['frate']; } else { echo 10; }?>"><br />
	Output video filename: <input type="text" id="filename-s" value="<?php echo sprintf("tl_%d.mp4", time()) ?>"><br />
	<input type="checkbox" id="heq">Use histogram equalization (may improve light/colour on badly exposed frames at the exepnse of overall quality)
</p>
<button id="gen_orig">Generate Orignal Timelapse</button>
<button id="gen_equi">Generate Equi Timelapse</button>
<?php
if(isset($_GET['tl'])) {
	if(empty($_GET['filename'])) {
		$tl_filename = sprintf("tl_%d.mp4", time());
	} else {
		$tl_filename = $_GET['filename'];
	}
	exec('/var/www/tl_gen.sh '.$_GET['ll'].' '.$_GET['ul'].' /var/www/tl_frames'.$_GET['tl'].' "/var/www/tl_exports/'.$tl_filename.'" '.$_GET['width'].' '.$_GET['frate'].' '.$_GET['histeq'].' > /dev/null &');
	echo("<p id='filename'>".$tl_filename."</p>");
}
?>
<br />
<p><a href="/tl_exports">View Exported Timelapse  Videos</a></p>
</body>
</html>
