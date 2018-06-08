const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>AlfaGT Command</title>
<link rel="icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
<link rel="shortcut icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
</head>
<body>
<nav class="blue-grey darken-4" role="navigation">
  <img id="logo-container" src="https://www.alfaromeousa.com/content/dam/alfausa/redesign/global/logo/alfa-romeo-logo-desktop-131x72-2.png" class="white brand-logo"></img>
  </nav>
  <div id="body">

<div class="centre-align boxy6">
<div class="centre-align card blue-grey darken-1">
<div class="centre-align card-action">
<h2 class="centre-align">ALFAGT COMMAND</h2>
  <h3>MODE</h3>
<button type="button" onclick="wd(1)">KITT MODE</button>
<button type="button" onclick="wd(4)">COP MODE</button>
<button type="button" onclick="wd(7)">MULTICOLOUR</button>
<button type="button" onclick="wd(0)">LIGHTS OFF</button><br>
<p>Temp Celcius is : <span id="alfaTemp" onload="temp()">0</span> C<br>
Relative Humidity is : <span id="alfaHumid" onload="humid()">NA</span> %<br>
MODE: <span id="watsdoin" onload="wd()">0</span><br>
DELAY: <span id="delay">30</span><br></p>
  <div class="boxy">
    <h4>DELAY</h4>
<button type="button" onclick="del(10)">10</button>
<button type="button" onclick="del(30)">30</button>
<button type="button" onclick="del(60)">60</button>
</div>  
  </div>
<script>
function wd(watsdoin) {
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
function del(delay) {
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

function humid() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("alfaHumid").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "humid", true);
xhttp.send();
}

function temp() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("alfaTemp").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "temp", true);
xhttp.send();
}

function mod() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("alfaMode").innerHTML =
this.responseText;
}
};
xhttp.open("GET", "mode", true);
xhttp.send();
}
setInterval(function() {
// Call a function repetatively
temp();
humid();
wd();
del();
}, 5000); //2000mSeconds update rate

</script>
<br><br>
</div>
</body>
</html>
)=====";
