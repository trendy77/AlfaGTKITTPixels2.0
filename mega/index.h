const char MAIN_page[] PROGMEM = R"=====(
  <head>
    <meta content="text/html; charset=utf-8"
          http-equiv="Content-Type">
    <meta content="width=device-width, initial-scale=1"
          name="viewport">
    <title>AlfaGT Command (min)</title>
  </head>
  <h1>AlfaGT COMMAND</h1>
  <div>
    <h3 style="color:blue;">CURRENT STATS</h3>
    <h6>TEMP (C): <span id="temp">0</span> C</h6>
    <h6>HUMIDITY (%): <span id="humid">NA</span> %</h6>
    <h6>MODE: <span id="watsdoin">1</span></h6>
    <h6>COLOUR:</h6>
    <p>Red:<span id="red">255</span></p>
    <p>Green:<span id="green">0</span></p>
    <p>Blue:<span id="blue">0</span></p>
    <h6>DELAY: <span id="delay"></span></h6>
    <h6>ROLLING PIX: <span id="rolling">False</span></h6>
    <h6>NEXT RUN:<span id="nextrun">NA</span> ms</h6><br>
    <p>CONSOLE: <span id="console">False</span></p>
  </div>
  <div>
    <h3 style="color:red;">MODE SELECT</h3><button onclick="roll(1)"
         type="button">ROLLING PIX</button><br>
    <button onclick="wd(1)"
         type="button">KITT MODE</button><br>
    <button onclick="wd(4)"
         type="button">COP MODE</button><br>
    <button onclick="wd(7)"
         type="button">MULTICOLOUR</button><br>
    <button onclick="wd(0)"
         type="button">LIGHTS OFF</button><br>
  </div>
  <div>
    <h3 style="color:red;">COLOUR SELECT</h3><button onclick="r(255)"
         type="button">SET Red</button> <button onclick="g(255)"
         type="button">SET Green</button> <button onclick="b(255)"
         type="button">SET Blue</button>
  </div>
  <div>
    <h3 style="color:red;">DELAY SELECT</h3><button onclick="del(10)"
         type="button">10</button><br>
    <button onclick="del(30)"
         type="button">30</button><br>
    <button onclick="del(60)"
         type="button">60</button><br>
  </div>
  <script>

    var Timer_UdpateMesures;
  /// Create a timer than update data every n secondes
    $('#tab_mesures').on('load-success',function (e,data){
      console.log("tab_mesures loaded");
      if ($('.nav-tabs .active > a').attr('href')=='#tab_mesures') {
        Timer_UdpateMesures=setTimeout(function(){
          updateMesures();
        },10000);
      }                 
    });   
        
    function updateMesures(){
      $.getJSON('/stats.json', function(data){
        //console.log("Mesures envoyees : " + JSON.stringify(data) + "|" + data.t + "|" + data.h + "|" + data.pa) ;
        $('#temp').html(data.t); 
        $('#up').html(data.up);
        $('#humid').html(data.h);
        $('#delay').html(data.d); 
        $('#watsdoin').html(data.w); 
        $('#rolling').html(data.r);
        $('#console').html(data.c);
        $('#red').html(data.rgb[0]);
        $('#green').html(data.rgb[1]);
        $('#blue').html(data.rgb[2]); 
      }).fail(function(err){
        console.log("err getJSON stats.json "+JSON.stringify(err));
      });
    };
  }
  </script>
<br><br>
</body>
</html>
)=====";

