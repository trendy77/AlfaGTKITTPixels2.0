const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>AlfaGT Command</title><link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
<link rel="icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
<link rel="shortcut icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
<style>h{ font-family: Helvetica;color: red;} p{color: white;} body{ background-image:url('http://4.bp.blogspot.com/-9YLLXboMOY8/ULYblJySkpI/AAAAAAAAEVA/7mQIS59JtxA/s1600/Alfa+Romeo+Logo+4.jpeg');background-repeat: no-repeat;background-position: center; background-attachment: fixed;background-position: center top; } </style>
</head><body style="background-color:black;">
<div class="w3-row w3-teal"><div class="w3-quarter">
<img id="logo-container" src="https://www.alfaromeousa.com/content/dam/alfausa/redesign/global/logo/alfa-romeo-logo-desktop-131x72-2.png" class="white brand-logo"></img>
</div><div class="w3-half" style="text-align:center">
<h1>AlfaGT COMMAND</h1>
</div></div><div class="w3-row w3-center"><p>Temp Celcius : <span id="temp" onload="gtemp()">0</span> C<br>Relative Humidity: <span id="humid" onload="ghumid()">NA</span> %</p>
</div><div class="w3-row w3-center"><div class="w3-quarter w3-container w3-dark-grey w3-center">
<h6>MODE @ <span id="watsdoin">NA</span></h6><br><br><h3 style="color:red;">MODE SELECT</h3>
<button type="button" class="w3-button w3-mobile" onclick="wd(1)">KITT MODE</button><br><button type="button" class="w3-button w3-mobile" onclick="wd(4)">COP MODE</button><br>
<button type="button" class="w3-button w3-mobile" onclick="wd(7)">MULTICOLOUR</button><br><button type="button" class="w3-button w3-mobile" onclick="wd(0)">LIGHTS OFF</button><br>
</div><div class="w3-quarter w3-container"><h2>w3-third</h2></div><div class="w3-quarter w3-container"><h2>w3-third</h2></div><div class="w3-quarter w3-container w3-dark-grey w3-center"><h6>DELAY @ <span id="delay">NA</span></h6><br><br><h3 style="color:red;">DELAY SELECT</h3>
<button type="button" class="w3-button w3-mobile" onclick="del(10)">10</button><br><button type="button" class="w3-button w3-mobile" onclick="del(30)">30</button><br><button type="button" class="w3-button w3-mobile" onclick="del(60)">60</button><br></div><div class="w3-row"><div class="w3-quarter w3-container"></div> 
</div><div class="w3-quarter w3-container w3-white"><h6>NEXT PIXRUN IN<br><span id="nextrun">NA</span> millis</h6><br></div></div>
<script>
function wd(watsdoin) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("watsdoin").innerHTML =
this.responseText;
}};
xhttp.open("GET", "watsdoin?watsdoin="+watsdoin, true);
xhttp.send();
}
function del(delay) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("delay").innerHTML =
this.responseText;
}};
xhttp.open("GET", "delay?delay="+delay, true);
xhttp.send();
}
function gtemp() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("temp").innerHTML =
this.responseText;
}};
xhttp.open("GET", "temp", true);
xhttp.send();
}
function ghumid() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("humid").innerHTML =
this.responseText;
}};
xhttp.open("GET", "humid", true);
xhttp.send();
}
function gnext() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("nextrun").innerHTML =
this.responseText;
}};
xhttp.open("GET", "nextrun", true);
xhttp.send();
}

setInterval(function() {
ghumid();
},5000);
setInterval(function() {
gnext();
},5000);
setInterval(function() {
gtemp();
}, 5000); //2000mSeconds update rate
</script>
<br><br>
</body>
</html>
)=====";

