<!DOCTYPE html>
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>AlfaGT Command</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.100.2/css/materialize.min.css" />
<link rel="stylesheet" href="https://trendypublishing.com.au/css/alfa.css" type="text/css" media="all" />
<link rel="icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
<link rel="shortcut icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
<script src="https://code.jquery.com/jquery-2.1.1.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.100.2/js/materialize.min.js"></script>
<script src="https://materializecss.com/js/init.js"></script>
</head>
<body>
<nav class="blue-grey darken-4" role="navigation">
<div class="container no-pad-right"><img id="logo-container" src="https://www.alfaromeousa.com/content/dam/alfausa/redesign/global/logo/alfa-romeo-logo-desktop-131x72-2.png" class="brand-logo">
</img>
</div>              
<button type="button" onclick="sendDelayD(10)">10</button>
<button type="button" onclick="sendDataD(30)">30</button>
<button type="button" onclick="sendDataD(60)">60</button>
</div>
<div class="col s12 m6">
<div class="card blue-grey darken-1">
<div class="card-action">
<h3>MODE</h3>
<button type="button" onclick="sendData(1)">KITT MODE</button>
<button type="button" onclick="sendData(4)">COP MODE</button>
<button type="button" onclick="sendData(7)">MULTICOLOUR MODE</button>
<button type="button" onclick="sendData(0)">LIGHTS OFF</button><br>
Temp Celcius is : <span id="alfaTemp">0</span> C<br>
Relative Humidity is : <span id="alfaHumid">NA</span> %<br>
MODE is currently @ <span id="watsdoin">0</span><br>
DELAY is currently @ <span id="delay">30</span><br>
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
