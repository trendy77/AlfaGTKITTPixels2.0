const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1"/>
<title>AlfaGT Command</title>
<link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
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
                    <li><a href="#stats">Stats</a></li>
                </ul>
     </div>
  </nav>
  <div id="index-banner" class="parallax-container">
    <div class="section no-pad-bot">
      <div class="container">
        <div class="container red-darken-4">
        <h1 class="header col s12 blue-grey darken-4-text">AlfaGT KITT COMMAND</h1>
  </div>
  <div class="container">
    <div class="collection">
			 <a href="#!" class="collection-item blue-grey darken-4" type="button" onclick="sendData(1)">KITT MODE</button></a>
		 <a href="#!" class="collection-item blue-grey darken-4" type="button" onclick="sendData(4)">COP MODE</button></a>
	 <a href="#!" class="collection-item align-center blue-grey darken-4" type="button" onclick="sendData(7)">MULTICOLOUR MODE</button></a>
	  <a href="#!" class="collection-item align-center blue-grey darken-4" type="button" onclick="sendData(0)">LIGHTS OFF</button></a>
 <a href="#!" class="collection-item align-right yellow blue-text" type="button" onclick="sendData(2)">Yellow</button></a>
	 <a href="#!" class="collection-item text-align-right blue" type="button" onclick="sendData(5)">Blue</button></a>
 <a href="#!" class="collection-item align-right green" type="button" onclick="sendData(8)">Green</button></a>
  		</div>

	    <div class="parallax"><img src="https://trendypublishing.com.au/css/arch.jpg" alt="Unsplashed background img 1"></div>
  </div>
  <div class="parallax-container">
    <div class="section">
      <!--   Icon Section   -->
      <div class="row">
        <div class="col s12 m4">
          <div class="icon-block">
            <h2 class="center brown-text"><i class="material-icons">flash_on</i></h2>
            <h5 class="center">STATS</h5>
       <p class="dark"><p class="white-text">	Temp Celcius : <span id="alfaTemp">0</span> C</p>
		 <p class="white-text"> Relative Humidity : <span id="alfaHumid">NA</span> %</p>	</p>
          <p class="white-text">MODE : <span id="watsdoin">0</span></p>
		<p class="white-text">DELAY @ <span id="delay">30</span></p>
				   </div>
        </div>
        <div class="col s12 m4">
          <div class="icon-block">
            <h2 class="center brown-text"><i class="material-icons">group</i></h2>
           <div class="collection">
                            <a href="#!" class="collection-item blue-grey darken-4-text" type="button" onclick="sendDelayD(10)">Set DELAY to 10</a>
                            <a href="#!" class="collection-item blue-grey darken-4-text" type="button" onclick="sendDataD(30)">Set DELAY to 30<span class="badge">Default</span></a>
                            <a href="#!" class="collection-item blue-grey darken-4-text" type="button" onclick="sendDataD(60)">Set DELAY to 60</a>
                        </div>
					</div>
        </div>
      </div>
    </div>
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
