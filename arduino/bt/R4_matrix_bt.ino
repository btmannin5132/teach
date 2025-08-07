/* 
 *  Bluetooth App for the Arduino R4 Wifi 
 *  Ben Manning, 2025
 *  Purdue University
 *   
 * I don't reccomend making a bunch of modifications to this, 
 * as you will also likely have to update the webpage that you are connecting to.
 *
 * Once the code is on the Arduino, go to the link: https://btmannin5132.github.io/teach/arduino/bt/bt-app.html or https://btmannin5132.github.io/teach/projects.html
 * DO NOT connect to the Arduino using your BT settings on your device.  The app will handle all of it.

 * Make sure BT is enabled on your device, but DO NOT connect to the arduino outside of the app.
 * Use the button on the site to connect to the arduino.

 */

#include <Arduino_LED_Matrix.h>
#include <ArduinoBLE.h>

// Unique UUIDs for our service and characteristic
#define SERVICE_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define CHARACTERISTIC_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"

// Create the BLE service and a writable characteristic
BLEService ledService(SERVICE_UUID);
BLECharacteristic commandCharacteristic(CHARACTERISTIC_UUID, BLERead | BLEWrite, 16); // Max 16 bytes for a command string

ArduinoLEDMatrix matrix;

// Current state of the LED matrix
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

void setup() {
  Serial.begin(9600);
  matrix.begin();

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("UNO R4 LED Matrix");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(commandCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();

  Serial.println("BLE LED Matrix ready. Waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (commandCharacteristic.written()) {
        
        // --- START OF CORRECTED COMMAND PROCESSING ---
        // Create a buffer to hold the incoming command as a C-style string
        char command_buffer[17];
        int len = commandCharacteristic.readValue(command_buffer, sizeof(command_buffer));
        command_buffer[len] = '\0'; // Manually null-terminate the string

        Serial.print("Received command: ");
        Serial.println(command_buffer);

        if (strcmp(command_buffer, "C") == 0) { // Clear
          for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 12; c++) {
              currentFrame[r][c] = 0;
            }
          }
          matrix.renderBitmap(currentFrame, 8, 12);
        } else if (strcmp(command_buffer, "S") == 0) { // Smile
          for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 12; c++) {
              currentFrame[r][c] = smileyFrame[r][c];
            }
          }
          matrix.renderBitmap(currentFrame, 8, 12);
        } else if (strcmp(command_buffer, "H") == 0) { // Heart
          for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 12; c++) {
              currentFrame[r][c] = heartFrame[r][c];
            }
          }
          matrix.renderBitmap(currentFrame, 8, 12);
        } else if (strcmp(command_buffer, "PurdueP") == 0) { // Purdue P
          for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 12; c++) {
              currentFrame[r][c] = purduePFrame[r][c];
            }
          }
          matrix.renderBitmap(currentFrame, 8, 12);
        } else if (strncmp(command_buffer, "P", 1) == 0) { // Pixel toggle
          // Parse the coordinates from the command_buffer
          int x = 0, y = 0;
          sscanf(command_buffer, "P%d,%d", &x, &y);
          
          if (x >= 0 && x < 12 && y >= 0 && y < 8) {
            currentFrame[y][x] = (currentFrame[y][x] == 0) ? 1 : 0;
            matrix.renderBitmap(currentFrame, 8, 12);
          }
        }
        // --- END OF CORRECTED COMMAND PROCESSING ---
        
      }
      BLE.poll();
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}