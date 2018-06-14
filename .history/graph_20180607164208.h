const char GRAPH_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1"/>
<title>AlfaGT Graphs</title>
  		<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.100.2/css/materialize.min.css" />
		<link rel="icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
		<link rel="shortcut icon" href="http://www.alfaromeo.com/favicon.ico" type="image/x-icon" />
</head>
<body>
  <nav class="blue-grey darken-4" role="navigation">
    <div class="nav-wrapper container">
        <div class="container no-pad align-right"><img id="logo-container" src="https://www.alfaromeousa.com/content/dam/alfausa/redesign/global/logo/alfa-romeo-logo-desktop-131x72-2.png" class="brand-logo"></img>
                </div>
 <ul class="left hide-on-med-and-down">
                    <li><a href="#cmd">Commands</a></li>
                    <li><a href="#graphs">Graphs</a></li>
                </ul>
     </div>
  </nav>
  <div id="index-banner" class="parallax-container">
    <div class="section no-pad-bot">
      <div class="container">
        <div class="container red-darken-4">
        <h1 class="header col s12 blue-grey darken-4-text">AlfaGT KITT GRAPHS</h1>
  </div>
  <div class="container">
   <div class="parallax"><img src="https://trendypublishing.com.au/css/arch.jpg" alt="Unsplashed background img 1"></div>
  </div>
  <div class="parallax-container">
    <div class="section">
      <!--   Icon Section   -->
      <div class="row">
        <div class="col s12 m4">
          <div class="icon-block">
       
       <p class="dark"><p class="white-text">	Temp Celcius : <span id="alfaTemp">0</span> C</p>
		 <p class="white-text"> Relative Humidity : <span id="alfaHumid">NA</span> %</p>	</p>
        

<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/1?bgcolor=%23000&color=%238000&dynamic=true&results=60&title=TEMP&type=column"></iframe>

<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/2?bgcolor=%23000&color=%2300f&dynamic=true&results=60&title=HUMIDITY&type=column"></iframe>

<iframe width="250" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/3?bgcolor=%23000&color=%23d62020&dynamic=true&results=60&title=KITTMODE&type=line"></iframe>
<iframe width="250" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/8?bgcolor=%23000&color=%23d62020&dynamic=true&results=60&title=Uptime&type=line"></iframe>
<iframe width="250" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/511863/charts/4?bgcolor=%23000&color=%23d62020&dynamic=true&results=60&title=Delay+%28lights%29&type=line"></iframe>

            
    <div class="footer-copyright">
      <div class="container black">Made with a little help & kudos to <a class="brown-text text-lighten-3" href="http://materializecss.com">Materialize</a> as well as <a class="blue-text text-lighten-3" href="http://alfaromeo.com">Alfa Romeo</a>.
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
</div>
</body>
</html>
)=====";
