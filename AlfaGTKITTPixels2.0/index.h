const char NOINT_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>AlfaGT Command (min)</title>


</head>
<body style="background-color:black;">
<h1>AlfaGT COMMAND</h1>

<div>
<h3 style="color:blue;">CURRENT STATS</h3>
<h6>TEMP (C): <span id="temp" onload="gtemp()">0</span> C</h6>
<h6>HUMIDITY (%): <span id="humid" onload="ghumid()">NA</span> %</h6>
<h6>MODE: <span id="watsdoin" onload="wd()">1</span></h6>
<h6>COLOUR:</h6>
<p>Red:<span id="r" onload="r()">255</span></p>
<p>Green:<span id="g" onload="g()">0</span></p>
<p>Blue:<span id="b" onload="b()">0</span></p>

<h6>DELAY: <span id="delay" onload="del()">10</span></h6>
<h6>ROLLING PIX: <span id="roll" onload="roll()">False</span></h6>
<h6>NEXT RUN:<span id="nextrun" onload="nextrun()">NA</span> ms</h6><br>
</div>

<div>
<h3 style="color:red;">MODE SELECT</h3>
<button type="button" onclick="roll(1)">ROLLING PIX</button><br>
<button type="button" onclick="wd(1)">KITT MODE</button><br>
<button type="button" onclick="wd(4)">COP MODE</button><br>
<button type="button" onclick="wd(7)">MULTICOLOUR</button><br>
<button type="button" onclick="wd(0)">LIGHTS OFF</button><br>
</div>

<div>
<h3 style="color:red;">COLOUR SELECT</h3>
<button type="button" onclick="r(255)">SET Red</button>
<button type="button" onclick="g(255)">SET Green</button>
<button type="button" onclick="b(255)">SET Blue</button>
</div>

<div>
<h3 style="color:red;">DELAY SELECT</h3>
<button type="button" onclick="del(10)">10</button><br>
<button type="button" onclick="del(30)">30</button><br>
<button type="button" onclick="del(60)">60</button><br>
</div>


 
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

function r(col) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("r").innerHTML =
this.responseText;
}};
xhttp.open("GET", "r?r="+col+"&b=0&g=0", true);
xhttp.send();
}

function g(col) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("g").innerHTML =
this.responseText;
}};
xhttp.open("GET", "g?g="+col+"&b=0&r=0", true);
xhttp.send();
}

function b(col) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("b").innerHTML =
this.responseText;
}};
xhttp.open("GET", "b?b="+col+"&r=0&g=0", true);
xhttp.send();
}

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
},25000);
setInterval(function() {
gnext();
},25000);
setInterval(function() {
gtemp();
}, 25000);
</script>
<br><br>
</body>
</html>
)=====";


const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>AlfaGT Command</title>
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css" />
<link rel="icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
<link rel="shortcut icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />

<style>
h{ font-family: Helvetica;color: red;} 
p{color: white;} 
body{ background-image:url('http://4.bp.blogspot.com/-9YLLXboMOY8/ULYblJySkpI/AAAAAAAAEVA/7mQIS59JtxA/s1600/Alfa+Romeo+Logo+4.jpeg');background-repeat: no-repeat;background-position: center; background-attachment: fixed;background-position: center top; } 
#form {   background-image:url('http://www.omniauto.it/awpImages/photogallery/2015/22601/photos1280/alfa-romeo-storia-logo-1907-2016__18.jpg'); 
   background-repeat: no-repeat;   background-attachment: fixed; background-position: center; opacity: 0.7;
  }  
#form.inv {  background-image: url('http://cdn.johnywheels.com/2016/03/02/alfaromeologoblackandwhite-l-ab8414f94741721d.jpg');  
  padding: 20px;  font-size: 25px;  text-align: center;  opacity:0.8;   }  
#form.dev {      background-image: url('https://i.pinimg.com/736x/5e/22/b6/5e22b67da60525879ea09b0960e12a2f.jpg');
   opacity:1;  }

.dropdown {    align:center;	position: relative;    display: inline-block;}
.dropdown-content {    display: none;    position: absolute;    background-color: #f1f1f1;    min-width: 160px;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);    z-index: 1;}
.dropdown-content a {    color: black;    padding: 12px 16px;    text-decoration: none;    display: block;}.dropdown-content a:hover {background-color: #ddd;}.dropdown:hover .dropdown-content {display: block;}
.dropdown:hover 
.dropbtn {background-color: #3e8e41;} 

</style>
</head>
<body style="background-color:black;">
<div class="w3-row w3-teal"><div class="w3-quarter">
<img id="logo-container" src="https://www.alfaromeousa.com/content/dam/alfausa/redesign/global/logo/alfa-romeo-logo-desktop-131x72-2.png" class="white brand-logo"></img>
</div><div class="w3-half" style="text-align:center">
<h1>AlfaGT COMMAND</h1>
</div>
<div class="w3-row">
  <div class="w3-third w3-container w3-dark-green">
    
<h3 style="color:blue;">CURRENT STATS</h3>
<h6>TEMP (C): <span id="temp" onload="gtemp()">0</span> C</h6>
<h6>HUMIDITY (%): <span id="humid" onload="ghumid()">NA</span> %</h6>
<h6>MODE: <span id="watsdoin" onload="gwd()">1</span></h6>
<h6>COLOUR:</h6>
Red:<span id="r" onload="r()">255</span><br>
Green:<span id="g" onload="g()">0</span><br>
Blue:<span id="b" onload="b()">0</span><br>
<h6>DELAYdel: <span id="delay" onload="del()">??</span></h6>
<h6>DELAYgde: <span id="delay" onload="gde()">??</span></h6>
<h6>ROLLING PIX: <span id="roll" onload="roll()">False</span></h6>
<h6>NEXT RUN:<span id="nextrun" onload="nextrun()">NA</span> ms</h6><br>
</div>
 </div>
  
  <div class="w3-third w3-container w3-dark-grey">
<div class="w3-row">
<button type="button" class="w3-button w3-mobile" onclick="wd(0)">PIX OFF</button>
<button type="button" onclick="roll(1)">ROLL PIX</button>
<br>
 <div class="dropdown">
  <button class="dropbtn">DELAY</button>
  <div class="dropdown-content">
   <a href="#" onclick="del(0)">NONE</a>
    <a href="#" onclick="del(10)">10</a>
  <a href="#" onclick="del(30)">30</a>
   <a href="#" onclick="del(60)">60</a>
  </div>
</div> 
 <div class="dropdown">
  <button class="dropbtn">MODE</button>
  <div class="dropdown-content">
   <a href="#" onclick="wd(1)">Standard KITT</a>
    <a href="#" onclick="wd(7)">Multi Colour</a>
  <a href="#" onclick="wd(2)">One Colour</a>
  </div>
</div> 
 <div class="dropdown">
  <button class="dropbtn">THEATRECHASERS</button>
  <div class="dropdown-content">
   <a href="#" onclick="wd(10)">TheatreChase</a>
<a href="#" onclick="wd(11)">theatreChaseRainbow</a>
<a href="#" onclick="wd(12)">colourWipe1</a>
<a href="#" onclick="wd(15)">colourWipe2</a>
<a href="#" onclick="wd(18)">colourWipeBlue</a>
  </div>
</div> 
 <div class="dropdown">
  <button class="dropbtn">COP MODE - CAUTION!</button>
  <div class="dropdown-content">
<a href="#" onclick="wd(4)">COPMODE</a>
<a href="#" onclick="wd(14)">COPMODE-WIPE</a>
  </div>
</div> 
 <div class="dropdown">
  <button class="dropbtn">NEW STYLEZ</button>
  <div class="dropdown-content">
<a href="#" onclick="wd(16)">rainbow2White</a>
<a href="#"  onclick="wd(17)">PulseWhite</a>
 <a href="#" onclick="wd(19)">WhiteOverRainbow</a>
  </div>
</div> 

<a href="#" onclick="r(255)">Red</a><br>
<a href="#" onclick="g(255)">Green</a><br>
<a href="#" onclick="b(255)">Blue</a><br>



<script>
function r(col) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("r").innerHTML =
this.responseText;
}};
xhttp.open("GET", "r?r="+col+"&b=0&g=0", true);
xhttp.send();
}

function g(col) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("g").innerHTML =
this.responseText;
}};
xhttp.open("GET", "g?g="+col+"&b=0&r=0", true);
xhttp.send();
}

function b(col) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("b").innerHTML =
this.responseText;
}};
xhttp.open("GET", "b?b="+col+"&r=0&g=0", true);
xhttp.send();
}

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
ghumid();
},15000);
setInterval(function() {
gnext();
},2000);
setInterval(function() {
gtemp();
}, 15000);

<br><br>
</body>
</html>
)=====";

