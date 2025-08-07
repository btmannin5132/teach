/* 
 *  Web App for the Arduino R4 Wifi 
 *  Ben Manning, 2025
 *  Purdue University
 *  
 *  SSID is "Arduino_LED"
 *  Password is "arduinoled"
 *  Both values can be changed  below
 *  
 *  If connecting on a phone, may need to move to airplane mode 
 *  depending on network settings on phone
 *  
 *  Once connected, in your browser, go to "http://192.168.4.1/"
 *  There are preset buttons, or the user can make whatever pattern they like
 *  
 *  To add more preset options: 
 *  
 *  1. make another frame variable
 *  2. Add button in the HTML area
 *  3. Update conditional list with the new frame
 *  4. Reupload, and refresh the page on the browser
 */


#include "WiFiS3.h"
#include "Arduino_LED_Matrix.h"

// Define the SSID and password for the network the Arduino will create
#define AP_SSID "Arduino_LED"
#define AP_PASS "arduinoled" // Must be at least 8 characters long

WiFiServer server(80);
ArduinoLEDMatrix matrix;


/*
 * FRAME Variables setup
 * 0 is off, 1 is on
 * 
 * byte frameName[8][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

byte testFrame[8][12] = {
    {0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}
};
 * 
 */
// The current state of the LED matrix

// These can be made in a compressed format using Hex, 
//I thought this would be more visually helpful
byte currentFrame[8][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Preset for a smiley face
byte smileyFrame[8][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Preset for a heart
byte heartFrame[8][12] = {
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Preset for a Purdue P
byte purduePFrame[8][12] = {
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}
};



const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Arduino LED Matrix Control</title>
<style>
body {
  font-family: Arial, sans-serif;
  background-color: #000000;
  color: #c7a456;
  text-align: center;
}
h1 {
  color: #c7a456;
}
#matrix {
  display: grid;
  grid-template-columns: repeat(12, 40px);
  grid-template-rows: repeat(8, 40px);
  gap: 5px;
  width: fit-content;
  margin: 20px auto;
  padding: 10px;
  background-color: #333333;
  border-radius: 10px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.5);
}
.led {
  width: 40px;
  height: 40px;
  background-color: #444444;
  border-radius: 50%;
  cursor: pointer;
  transition: background-color 0.2s;
  border: 2px solid #333333;
}
.led.on {
  background-color: #c7a456;
  border: 2px solid #c7a456;
  box-shadow: 0 0 10px #c7a456;
}
.controls {
  margin-top: 20px;
}
button {
  padding: 10px 20px;
  font-size: 16px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  background-color: #c7a456;
  color: black;
  margin: 5px;
  transition: background-color 0.2s;
}
button:hover {
  background-color: #b3934f;
}
.footer-content {
  margin-top: 40px;
  padding: 20px;
  border-top: 2px solid #c7a456;
  text-align: left;
  font-size: 14px;
  max-width: 800px;
  margin-left: auto;
  margin-right: auto;
}
.footer-content h2 {
  color: #c7a456;
  margin-top: 0;
}
.footer-content p {
  color: #ecf0f1;
  line-height: 1.6;
}
</style>
</head>
<body>

<h1>Arduino LED Matrix Control</h1>

<!-- 
Add the button here inside the div section.

The 'function name will be what is called in the code below
button text will be what is shown on the button, keep it to 10 or less characters

<button onclick="loadPreset('functionName')">buttonText</button> 
 -->
<div class="controls">
  <button onclick="clearMatrix()">Clear</button>
  <button onclick="loadPreset('smile')">Smile</button>
  <button onclick="loadPreset('heart')">Heart</button>
  <button onclick="loadPreset('purdueP')">Purdue P</button>
</div>
<div id="matrix"></div>

<div class="footer-content">

  <p>&copy; 2025 Ben Manning</p>
</div>

<script>
document.addEventListener('DOMContentLoaded', () => {
  const matrix = document.getElementById('matrix');
  
  for (let y = 0; y < 8; y++) {
    for (let x = 0; x < 12; x++) {
      const led = document.createElement('div');
      led.className = 'led';
      led.dataset.x = x;
      led.dataset.y = y;
      led.addEventListener('click', () => toggleLed(x, y, led));
      matrix.appendChild(led);
    }
  }
});

function toggleLed(x, y, ledElement) {
  ledElement.classList.toggle('on');

  const url = `/led?x=${x}&y=${y}`;
  fetch(url).catch(error => console.error('Network error:', error));
}

function clearMatrix() {
  const url = '/clear';
  fetch(url)
    .then(() => {
      document.querySelectorAll('.led').forEach(led => led.classList.remove('on'));
    })
    .catch(error => console.error('Network error:', error));
}

function loadPreset(preset) {
  const url = `/preset?name=${preset}`;
  fetch(url)
    .then(response => response.json())
    .then(data => {
      document.querySelectorAll('.led').forEach((led, index) => {
        const x = parseInt(led.dataset.x);
        const y = parseInt(led.dataset.y);
        if (data[y][x] === 1) {
          led.classList.add('on');
        } else {
          led.classList.remove('on');
        }
      });
    })
    .catch(error => console.error('Network error:', error));
}
</script>

</body>
</html>
)=====";

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.renderBitmap(currentFrame, 8, 12);

  Serial.print("Creating access point named: ");
  Serial.println(AP_SSID);

  int status = WiFi.beginAP(AP_SSID, AP_PASS);

  if (status == WL_AP_LISTENING) {
    Serial.println("Access Point created successfully!");
    Serial.print("Webpage IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.print("Creating access point failed: ");
    Serial.println(status);
    while(true);
  }

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            if (request.indexOf("GET /led") != -1) {
              int x = -1, y = -1;
              int xIndex = request.indexOf("x=");
              int yIndex = request.indexOf("y=");
              
              if (xIndex != -1 && yIndex != -1) {
                x = request.substring(xIndex + 2).toInt();
                y = request.substring(yIndex + 2).toInt();
              }
              
              if (x >= 0 && x < 12 && y >= 0 && y < 8) {
                currentFrame[y][x] = (currentFrame[y][x] == 0) ? 1 : 0;
                matrix.renderBitmap(currentFrame, 8, 12);
              }
              
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: text/plain");
              client.println();
              client.println("LED Toggled");
            } else if (request.indexOf("GET /clear") != -1) {
              // Clear the entire frame
              for(int r = 0; r < 8; r++) {
                for(int c = 0; c < 12; c++) {
                  currentFrame[r][c] = 0;
                }
              }
              matrix.renderBitmap(currentFrame, 8, 12);
              
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: text/plain");
              client.println();
              client.println("Matrix cleared");
            } else if (request.indexOf("GET /preset?name=") != -1) {
              // Load a preset image
              String presetName = request.substring(request.indexOf("name=") + 5, request.indexOf(" HTTP/1.1"));
              
              if (presetName == "smile") {
                for (int r = 0; r < 8; r++) {
                  for (int c = 0; c < 12; c++) {
                    currentFrame[r][c] = smileyFrame[r][c];
                  }
                }
              } else if (presetName == "heart") {
                for (int r = 0; r < 8; r++) {
                  for (int c = 0; c < 12; c++) {
                    currentFrame[r][c] = heartFrame[r][c];
                  }
                }
              } else if (presetName == "purdueP") {
                for (int r = 0; r < 8; r++) {
                  for (int c = 0; c < 12; c++) {
                    currentFrame[r][c] = purduePFrame[r][c];
                  }
                }
              }

/*              
 *               Add an elseif for the preset that you want to make
 *               
 *               Not the prettiest way to do this, but thought it would be the best to demonstrate
 *               
 *               Change the "preset name" to the function call made in the HTML section
 *               Change the "testFrame" to the screen variable made at the top
 *               
 
              else if (presetName == "funcName") {
                for (int r = 0; r < 8; r++) {
                  for (int c = 0; c < 12; c++) {
                    currentFrame[r][c] = testFrame[r][c];
                  }
                }
              }

 */
              matrix.renderBitmap(currentFrame, 8, 12);
              
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: application/json");
              client.println();
              client.print("[");
              for(int r = 0; r < 8; r++) {
                client.print("[");
                for(int c = 0; c < 12; c++) {
                  client.print(currentFrame[r][c]);
                  if(c < 11) client.print(",");
                }
                client.print("]");
                if(r < 7) client.print(",");
              }
              client.println("]");

            } else {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: text/html");
              client.println();
              client.println(webpage);
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}