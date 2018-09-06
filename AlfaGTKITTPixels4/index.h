const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />

<script>
function roll(yay) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("rolling").innerHTML =
this.responseText;
}};
xhttp.open("GET", "rolling?rolling="+yay, true);
xhttp.send();
}

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
function del(Ddelay) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("delay").innerHTML =
this.responseText;
}};
xhttp.open("GET", "delay?delay="+Ddelay, true);
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
function gde() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("delay").innerHTML =
this.responseText;
}};
xhttp.open("GET", "delay", true);
xhttp.send();
}
function gwd() {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("watsdoin").innerHTML =
this.responseText;
}};
xhttp.open("GET", "watsdoin", true);
xhttp.send();
}
setInterval(function() {
roll();
gwd();
gde();
},15000);

<br><br>
</body>
</html>
)=====";

