const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<link rel='shortcut icon' type='image/ico' href='http://www.alfaromeo.com/favicon.ico' />
<script src="https://code.jquery.com/jquery-2.1.1.min.js"></script>
<link rel='stylesheet' type='text/css' href='https://trendypublishing.com.au/css/alfa.css'></link>
<title>AlfaGT Command</title>
</head>
<body>
<div id="body">
    <div id='form' class='main'>
<div id='form' class='header'>
       	   <h1 align="center">ALFAGT COMMAND</h1>
<img src="https://www.alfaromeousa.com/content/dam/alfausa/redesign/global/logo/alfa-romeo-logo-desktop-131x72-2.png"></img>
       </div>
              <div id='form' class='inverse'>
				<div id='form' class='main'>
       <div id='form' class='header'>
                         <h3>MODE</h3>
       </div>
	   <button type="button" onclick="sendData(1)">KITT MODE</button>
                    <button type="button" onclick="sendData(4)">COP MODE</button>
                    <button type="button" onclick="sendData(7)">MULTICOLOUR MODE</button>
                    <button type="button" onclick="sendData(0)">LIGHTS OFF</button>
					<BR>
                    <button type="button" onclick="sendData(2)">Yellow</button>
                       <button type="button" onclick="sendData(5)">Blue</button>
                       <button type="button" onclick="sendData(8)">Green</button>
                    <BR>
					
<div id='form' class='header'>
   <h3>DELAY</h3>
</div>
	   <button type="button" onclick="sendDelayD(10)">10</button>
                    <button type="button" onclick="sendDataD(30)">30</button>
                    <button type="button" onclick="sendDataD(60)">60</button>
                   	<BR>
                     </div>
                </div>
                   <div id='form' class='inverse'>
	   <div id='form' class='main'>
	   <div id='form' class='header'>
                <h3>STATS</h3>
				</div>
                MODE is currently @ <span id="watsdoin">0</span><br>
				DELAY is currently @ <span id="delay">30</span><br>
 <div id="graph">               Temp Celcius is : <span id="alfaTemp">0</span> C<br>
				<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/1?bgcolor=%23000&color=%238000&dynamic=true&results=60&title=TEMP&type=column"></iframe>
				<iframe width="300" height="260" style="border: 10px solid #cccccc;" src="https://thingspeak.com/channels/511863/widgets/3359"></iframe>
<br>
 Relative Humidity is : <span id="alfaHumid">NA</span> %<br>
 <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/2?bgcolor=%23000&color=%2300f&dynamic=true&results=60&title=HUMIDITY&type=column"></iframe>
		<iframe width="400" height="260" style="border: 10px solid #cccccc;" src="https://thingspeak.com/channels/511863/widgets/3358"></div>
 </div>
			<br>
<div id="graph2">
			<iframe width="250" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/3?bgcolor=%23000&color=%23d62020&dynamic=true&results=60&title=KITTMODE&type=line"></iframe>
			<iframe width="250" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/8?bgcolor=%23000&color=%23d62020&dynamic=true&results=60&title=Uptime&type=line
			<iframe width="250" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/4?bgcolor=%23000&color=%23d62020&dynamic=true&results=60&title=Delay+%28lights%29&type=line"></iframe>
		</div>
		</div>
        </div> 
<script>
function sendData(watsdoin) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("watsdoin").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "watsdoin?watsdoin="+watsdoin, true);
xhttp.send();
}
function sendDataD(delay) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("delay").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "delay?delay="+delay, true);
xhttp.send();
}

function getHumid() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("alfaHumid").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "getHumid", true);
xhttp.send();
}

function getWd() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("watsdoin").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "getDoin", true);
xhttp.send();
}
setInterval(function() {
// Call a function repetatively
getTemp();
getHumid();
getWd();
}, 5000); //2000mSeconds update rate

function getTemp() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("alfaTemp").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "getTemp", true);
xhttp.send();
}
</script>
<br><br>
</body>
</html>
)=====";
