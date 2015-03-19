function HTTPGET(url) {
	var req = new XMLHttpRequest();
	req.open("GET", url, false);
	req.send(null);
	return req.responseText;
}

var playSound = function() {
	
	//call php script on webserver (raspberry pi) to play sound
	var response = HTTPGET("http://10.10.0.25/dalek/play_exterminate.php");
		
};

Pebble.addEventListener("ready",
  function(e) {
	//App is ready to receive JS messages
	playSound();
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	//Watch wants new data!
	playSound();
  }
);
