#ifndef home_h
#define home_h

const char home[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <!-- CSS only -->
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@flaticon/flaticon-uicons@3.1.0/css/all/all.min.css">

  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css"
    integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">

  <title>FlowerWatch - növényfigyelő</title>

  <style>
    .adatok {
      margin-top: 15px;
      /* border: 1px black solid; */
      line-height: 330%;
    }

    .col-4 {
      border: 1px black solid;
      border-top: 0px;
    }

    .col-8 {
      border: 1px black solid;
      border-top: 0px;
      border-right: 0px;

    }

    .also {
      border-bottom: 0px;
      margin-bottom: 15px;
    }

    .noborder {
      border: 0px;
    }

    .noborder_left {
      border-left: 0px;
    }

    .noborder_right {
      border-right: 0px;
    }

    .ikon {
      /*border: 1px solid black;*/
      margin-right: 3px;
      padding-top: 10px;
    }

    .col-4 {
      border-right: 0px;
    }

    .col-8 {
      border-left: 0px;
    }
  </style>
</head>

<body>
  <div>
    <nav class="navbar navbar-expand-lg bg-dark" data-bs-theme="dark">
      <div class="container-fluid">
        <a class="navbar-brand" href="/">FlowerWatch</a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarColor02"
          aria-controls="navbarColor02" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarColor02">
          <ul class="navbar-nav me-auto">
            <li class="nav-item">
              <a class="nav-link active" target="_blank" href="https://github.com/Excal21/FlowerWatch">GitHub
                <span class="visually-hidden">(current)</span>
              </a>
            </li>
            <li class="nav-item dropdown">
              <a class="nav-link dropdown-toggle" data-bs-toggle="dropdown" href="#" role="button" aria-haspopup="true"
                aria-expanded="false">Forráslinkek</a>
              <div class="dropdown-menu">
                <a class="dropdown-item" target="_blank" href="https://www.flaticon.com/uicons">Flaticon</a>
                <a class="dropdown-item" target="_blank" href="https://github.com/tzapu/WiFiManager">Wifi Manager (tzapu) -
                  GitHub</a>
                <a class="dropdown-item" target="_blank" href="https://github.com/adafruit/Adafruit_MCP3008">Adafruit
                  MCP3008 - GitHub</a>
                <a class="dropdown-item" target="_blank" href="https://github.com/beegee-tokyo/DHTesp">DHTEsp (beegee-tokyo) - GitHub</a>
                <a class="dropdown-item" target="_blank" href="https://github.com/mobizt/ESP-Mail-Client">ESP Mail
                  Client (mobizt) - GitHub</a>
                <a class="dropdown-item" target="_blank"
                  href="https://randomnerdtutorials.com/esp32-cam-send-photos-email/">Random Nerd Tutorials - Take and
                  Send Photos via Email</a>
            </li>
          </ul>
        </div>
      </div>
    </nav>
  </div>


  <div class="container-fluid adatok">

    <div class="row">
      <div class="col-8">
        <i style="font-size: xx-large; color: rgb(217, 92, 43);" class="fi fi-rr-summer ikon align-middle"></i><i
          style="font-size: x-large;" class=" align-middle"> Hőmérséklet</i>
      </div>
      <div class="col-4">
        <i style="font-size: x-large;" id="temp"></i>
      </div>
    </div>

    <div class="row">
      <div class="col-8">
        <i style="font-size: xx-large; color:rgb(42, 156, 87);" class="fi fi-rs-raindrops ikon align-middle"></i><i
          style="font-size: x-large" class="align-middle"> Nedvesség</i>
      </div>
      <div class="col-4">
        <i style="font-size: x-large;" id="soil"></i>
      </div>
    </div>

    <div class="row">
      <div class="col-8">
        <i style="font-size: xx-large; color: rgb(24, 161, 219);" class="fi fi-rs-humidity ikon align-middle"></i><i
          style="font-size: x-large;" class="align-middle"> Páratartalom</i>
      </div>
      <div class="col-4">
        <i style="font-size: x-large;" id="humidity"></i>
      </div>
    </div>

    <div class="row">
      <div class="col-8 also">
        <i style="font-size: xx-large;  color: rgb(247, 223, 5);" class="fi fi-rr-brightness ikon align-middle"></i><i
          style="font-size: x-large;" class="align-middle"> Fény</i>
      </div>
      <div class="col-4 also">
        <i style="font-size: x-large;" id="light"></i>
      </div>
    </div>


  </div>



  <div class="container-fluid">
    <form action="/get">
      <fieldset class="form-group">
        <legend class="mt-2 also" style="font-size: x-large;">Beállítások</legend>
        <div class="form-check form-switch also" style="float: left">
          <input class="form-check-input" type="checkbox" id="ledcheckbox" onclick="updateLED()">
          <input type="hidden" id="prefled" name="ledin" value="1">
          <label class="form-check-label" for="flexSwitchCheckChecked">Visszajelző LED</label>
        </div>
        <div class="form-check form-switch also" style="float: left; margin-left: 20px;">
          <input class="form-check-input" type="checkbox" id="flashcheckbox" onclick="updateLED()">
          <input type="hidden" id="prefflash" name="flashin" value="1">
          <label class="form-check-label" for="flexSwitchCheckChecked">Vaku</label>
        </div>
      </fieldset>
      <fieldset class="form-group">
        <label class="form-label" style="margin-right: 10px;">Földnedvesség:</label><label id="soilval"> 30%</label>
        <input name="soilin" type="range" class="form-range" min="0" max="100" step="1" value="30" id="soilmin">

        <label class="form-label" style="margin-right: 10px;">Maximum hőmérséklet:</label><label id="tempval"> 40
          °C</label>
        <input name="tempin" type="range" class="form-range also" min="0" max="50" step="1" value="40" id="tempmax">
      </fieldset>


      <fieldset>
        <div class="form-group row">


          <div class="col-4 noborder">

            <label class="col-form-label">Email</label>
          </div>
          <div class="col-8 noborder">

            <input type="text" readonly="" class="form-control-plaintext" id="prefemail" value="example@example.com">
          </div>
        </div>



        <div class="form-group also">
          <input name="emailin" type="email" class="form-control" style="margin-bottom: 20px;" id="exampleInputEmail1"
            aria-describedby="emailHelp" placeholder="Ezen a címen fog értesíteni a növényfigyelő">
        </div>



        <input type="submit" class="btn btn-primary also" value="Beállít" href="/">
        <button type="button" class="btn btn-primary also" style="margin-left: 10px;" onclick="sendPhoto()">Fotó
          küldése</button>
      </fieldset>
    </form>
  </div>


  <script>

    var soilrangeInput = document.getElementById("soilmin");
    var soilrangeValue = document.getElementById("soilval");


    soilrangeInput.addEventListener("input", function () {
      soilrangeValue.textContent = soilrangeInput.value + " %";
    });

    var temprangeInput = document.getElementById("tempmax");
    var temprangeValue = document.getElementById("tempval");

    temprangeInput.addEventListener("input", function () {
      temprangeValue.textContent = temprangeInput.value + " °C";
    });




    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    // Init web socket when the page loads
    window.addEventListener('load', onload);

    function onload(event) {
      initWebSocket();
    }

    function getReadings() {
      websocket.send("getReadings");
    }

    function sendPhoto() {
      websocket.send("sendPhoto");
      alert("Kép küldése folyamatban, kérlek várj!");
    }

    function initWebSocket() {
      console.log('Trying to open a WebSocket connection…');
      websocket = new WebSocket(gateway);
      websocket.onopen = onOpen;
      websocket.onclose = onClose;
      websocket.onmessage = onMessage;
    }

    // When websocket is established, call the getReadings() function
    function onOpen(event) {
      console.log('Connection opened');
      websocket.send("getPreferences");
      getReadings();
    }

    function onClose(event) {
      console.log('Connection closed');
      setTimeout(initWebSocket, 2000);
    }

    function updateLED() {
      var checkbox = document.getElementById('ledcheckbox');
      var hiddenInput = document.getElementById('prefled');

      var checkbox2 = document.getElementById('flashcheckbox');
      var hiddenInput2 = document.getElementById('prefflash');


      if (checkbox.checked) {
        hiddenInput.value = 1;
      } else {
        hiddenInput.value = 0;
      }


      if (checkbox2.checked) {
        hiddenInput2.value = 1;
      } else {
        hiddenInput2.value = 0;
      }
    }

    // Function that receives the message from the ESP32 with the readings
    function onMessage(event) {
      console.log(event.data);
      var myObj = JSON.parse(event.data);
      var keys = Object.keys(myObj);

      for (var i = 0; i < keys.length; i++) {
        var key = keys[i];

        switch (key) {
          case "prefemail":
            document.getElementById("prefemail").value = myObj[key];
            break;
          case "prefled":
            if (myObj[key] === 1) {
              document.getElementById("ledcheckbox").checked = true;
            }
            else {
              document.getElementById("ledcheckbox").checked = false;
            }
            updateLED();
            break;
          case "prefflash":
            if (myObj[key] === 1) {
              document.getElementById("flashcheckbox").checked = true;
            }
            else {
              document.getElementById("flashcheckbox").checked = false;
            }
            updateLED();
            break;


          case "prefsoilmin":
            document.getElementById("soilmin").value = myObj[key];
            document.getElementById("soilval").innerHTML = myObj[key] + " %";
            break;
          case "preftempmax":
            document.getElementById("tempmax").value = myObj[key];
            document.getElementById("tempval").innerHTML = myObj[key] + " °C";
            break;

          case "temp":
            document.getElementById(key).innerHTML = parseFloat(myObj[key]).toFixed(1) + " °C";
            break;
          case "humidity":
            document.getElementById(key).innerHTML = parseFloat(myObj[key]).toFixed(1) + " %";
            break;
          case "soil":
            document.getElementById(key).innerHTML = parseFloat(myObj[key]).toFixed(1) + " %";
            break;
          case "light":
            document.getElementById(key).innerHTML = parseFloat(myObj[key]).toFixed(1) + " lx";
            break;
        }
      }
    }



  </script>


  <!-- JavaScript Bundle with Popper -->
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js"
    integrity="sha384-C6RzsynM9kWDrMNeT87bh95OGNyZPhcTNXj1NW7RuBCsyN/o0jlpcV8Qyq46cDfL"
    crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/@flaticon/flaticon-uicons@3.1.0/license.min.js"></script>
</body>

</html>)rawliteral";


#endif