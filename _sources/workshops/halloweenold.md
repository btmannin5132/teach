---
layout: page
title: Course Development
permalink: /workshops/halloween
---

# Halloween Animatronics Workshop

This workshop was designed to introduce students to animatronics during the Halloween season.

Animatronics are often seen as these complex, human-like robots that can either act like animals, or at least sing a song on stage.  

The main goal of this workshop is to show that you don't need any crazy equipment or tools, or even a lot of inital money, to get started in animatronics or robotics in general.  The neat thing about a lot of simple robotics and animatronics projects is that the electronics and code don't actually change a whole lot.  The example we are using in this workshpop can be applied to a wide variety of other animatronics that one would want to react to light or movement.  The most time-consuming part, and arguably the most fun part, is developing the shells and enviornments that your animatronics will work in. The giant skeletons in a front yard can be setup in a very similar way, the main difference being you will likely need some bigger motors, some rigging for the actuation of the arms and head, and perhaps a weatherproof enclosure for your electronics.

## End product

The end product of this workshop will be a small animatronic diorama of a haunted house/grave/tree or whatever setting you deside to make where a ghost/spider/bat/other creepy figure will pop out of the setting when someone or something moves past it.

<video src="_static/assets/videos/halloween1.mp4" width="320" height="240" controls></video>

## Materials Needed

Ideally, this project can be completed for less than $10 USD.  Generic Arduino Nanos go for about $3 on your prefered online seller when you purchase packs of 3 or more, so you will have more ready for even more projects!

### General tools

- Scissors
- Hot glue Gun
- Computer with the [Arduino IDE installed](https://www.arduino.cc/en/software/)
- USB cable to connect with Arduino (Arduino Nano uses USB-mini , micro, or C dependning on model)

### Consumables

- Tissues, pipe cleaners, cotton balls, or whatever else you want to make your creepy figure with
- Arduino control board, I will be using an Arduino Nano
- jumper wires
- 1 photo-resistor
- 1 10KOhm Resistor
- 1 micro servo
- 1 paperclip or stiff wire
- 1 small box that you can cut
- Hot glue
- Tape

## Mechanical Construction

### Making the Diorama

Making the diorama will vary depending on what type of box you have, and how large you choose to make it.  I had a small clothing box that had a detachable lid.  I decided to just use the lid, and fold half of it up to make both a floor and front that I can then cut to whatever shape I like.

I made a cut along the sides of the lid to allow for simpiler folding, folded to a right angle, and then taped it in place.

After the folding, I cut my box into the 2d profile of a house, added some drawings and called it good for the sake of demonstration.  Feel free to get as creative as you like with this!  

### Making the Figre

For the sake of demonstration, I made a simple ghost that will pop out of my house. This figure can be whatever you like though.  This needs to stay fairly lightweight so the motor can move it quickly, but most microservos can move about 8 ounces without much issue, so feel free to get creative.

For the ghost, I balled up a few tissues into a small ball, and then laid another tissue over the ball to make it look like a flowing sheet.  I used a spare zip tie to loosely secure the tissue to the ball, and drew a face on it.

## Electrical Construction

The electrical side of this project has 2 main systems that need to talk to each other.  We have a servo that is moving the figure in and out of view, and we have a photoresistor that is telling the servo when to do so.  This is all being handled by an Arduino, which will take input data from the photoresistor, and then send a signal to the servo to move when the light changes appropriately.

### The photoresistor circuit

A resistor inside of a circuit has one job, and that is to resist electical current inside of that circuit.  That limitation of current may sound un-helpful, but it allows us to do some pretty awesome things.

A photoresistor is a type of resistor that changes how much it resists that electrical current based on how much light is hitting it.  If it is very dark around the photoresistor, the photoresistor has a very high resistance, sometimes on the order of a MegaOhm.  When it is very bright, the resistance decreases signifcantly, and can often just be in the tens of Ohms. The photoresistor relies on some neat physics in order to do this, so feel free to check that out if you are currious.

The tricky part about this is Arduinos can't measure current, they can only measure changes in voltage, so in order to make the photoresistor "output a voltage" instead, we are going to setup the photoresistor using one of my personal favorite types of circuits, a voltage divider.  A voltage divider uses 2 resistors to provide a voltage value that is related to the ratio of the two resisor values.  This is thanks to **Kirchoff's Voltage Law** if you are currious, feel free to explore this more if you like.  

Simply stated, Kirchoff's voltage law tells us that the voltage provided to a circuit gets split up between all of the components.  The larger the resistance of a device related to the rest of the components in the circuit, the more voltage the device takes.  This means that the photoresistor will change how much voltage it takes in our voltage divider circuit, and we can measure that change.

<img src="_static/assets/images/photoVD.png"  height="240">

### The Servo Motor

A servo is a special type of motor that turns to a specific anlge depending on the signal that is sent to it.  This is usually a **Pulse Width Modulation** value or **PWM** that tells the servo to turn to a specific angle.  We can use this to move our figure up and down in our diarama.

### Code

To monitor this change using an Arduino, you can use the sample code here.  This will use `analogRead` to take a voltage measurement of the circuit above that is connected to `A0` on the Arduino, and then print that reading out to the **serial monitor**.  You can open the serial monitor by pressing `ctrl`+`shift`+`m` on your keyboard.  The value you will see will range between 0 and 1023.  This is because the value outputs a 10 bit number, and essentially chops up 5V into 1023 pieces to give a precice voltage reading without needing to worry about deciminal places.

```cpp
//Analog input reading on an Arduino
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);


  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
  ```

Using the code and the circuit above, as it gets darker around the photoresistor, the value printed out will get smaller.  This is because we are reading the voltage accross the 10K resistor on the bottom of the circuit instead of the photoresistor.  As it gets darker, the photoresistor increases resistance, which means it will take more of the alloted voltage for the circuit, leaving less for the 10k resistor on the bottom.  As more light hits the photoresistor, the resistance decreases, meaing that the voltage accross the 10k resistor will increase.

Using this idea, we can setup a contitional statement that can determine if it is light or dark out

```cpp
//Analog input reading on an Arduino
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value output to the PWM (analog out)

int darkThreshold = 400;  //If the value from the sensor is less than this value, we will say that it is dark out.  Feel free to adjust as needed

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  // print the results to the Serial Monitor:

  if (sensorValue < darkThreshold){
    Serial.println("Dark!");
  }
  else{
    Serial.println("Light!");
  }
  Serial.print("sensor = ");
  Serial.println(sensorValue);


  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
```

Now that we have a way to monitor the light in the area, we need to tell the servo to move accordingly.  The `Servo` library in the Arduino IDE will help with this significantly as we can jsut put in the angle that we want the servo to move to, and the `Servo` library will convert the value to the proper PWM value needed.

```cpp
#include <Servo.h> //Use the Servo library to make using the servo simpler


// Setup variables
Servo servo;

int photo = A7;// Photoresistor pin
int photState = 0; // a variable to store the value of the photoresistor

//Position variables for the servo, these might need to be adjusted slightly for each project
int pos1 = 120; 
int pos2 = 0;

//Initial trheshold for the light sensor
int threshold = 400;

void setup() {

 //Servopin
servo.attach(2);
servo.write(pos1);

Serial.begin(9600);

setThresh(); // Determine Ambient Light level

}

void loop() {
  photState = analogRead(photo); //Read value from photoresistor
  Serial.println(photState); //Print out for troubleshooting

  if(photState > threshold){ //If the sensor is reading a low level of light, trigger the servo (else statement)
    //The .9 is there to give a bit of lenience on the threshold so the figure isn't just randomly popping up and down.
    servo.write(pos2);
  }
  else{
    servo.write(pos1);
  }
  // Give the arduino a quick break before resuming the loop
  delay(100);

}


```

That's all we need to run the diorama!  Now let's put everything together

## Puting it all together

1. Attach your figure to the end of a stretched out paperclip or a stiff wire (I would reccomend hot glue or tape), and attach the other side of the wire to one of the little arms that comes with the servo.The paper clip should be snug on both parts, but if it wiggles a bit, that is okay, and might even add some character to the figure if it shakes a bit.

2. Now glue your servo to your box so that when it moves, the figure pops up over the top of your diorama.  

3. Poke a hole in the front of your diorama so that the photoresistor can stick out.  Glue the photoresistor in place, but make sure you still have clean leads on the photoresistor to that it can still have good electrical connections.  

4. According to the code above, the photoresistor is going to be read on `A7` on the Arduino Nano, so we need to attach it ther.  One leg of the photoresistor should be attached to `5V` on the Arduino, and the other leg should be attached to `A7`.  Take your 10k resistor, and attach one lead to `A7` and the other lead to `GND`  Use your jumper wires to make these connections longer as needed.

5. Your servo has 3 wires that need to get connected to the arduino.  The `RED` wire needs to be attached to `5V`, the `YELLOW` wire should be attached to pin `2` (or `D2` on other Arduinos), and the `BROWN` or `BLACK` wire should be attached to `GND`.

After all of these connections are made properly, and the Arduino has the program uploaded, you should start seeing the servo move, hopefully with your figure!

## Great job!  

That is all there is to it to start with Animatronics!  Now this is a fairly simple animatronic process, and there are a lot of things we can do to make it more complex or more reliable. We can add lights, more motors and figures, speakers, whatever you like to make the diorama more interactive and exciting.  If you like, the below code takes a reading of the light around the photoresistor before starting the servo, and adjusts the threshold to whatever the ambient light level is in the area.  This means we can use it in very bright rooms, or in dimly lit rooms without needing to change the code!

```cpp
#include <Servo.h> //Use the Servo library to make using the servo simpler

// Setup variables
Servo servo;

int photo = A7;// Photoresistor pin
int photState = 0; // a variable to store the value of the photoresistor

//Position variables for the servo, these might need to be adjusted slightly for each project
int pos1 = 120; 
int pos2 = 0;

//Initial trheshold for the light sensor
int threshold = 400;

void setup() {

 //Servopin
servo.attach(2);
servo.write(pos1);

Serial.begin(9600);
}

void loop() {
  photState = analogRead(photo); //Read value from photoresistor
  Serial.println(photState); //Print out for troubleshooting

  if(photState > threshold*.9){ //If the sensor is reading a low level of light, trigger the servo (else statement)
    //The .9 is there to give a bit of lenience on the threshold so the figure isn't just randomly popping up and down.
    servo.write(pos2);
  }
  else{
    servo.write(pos1);
  }
  // Give the arduino a quick break before resuming the loop
  delay(100);

}


// Determine ambient light level
void setThresh(){

long temp = 0;

//Take average value over 500 readings to determine ambient light level
for (int i = 0; i < 500; i++){
  temp += analogRead(photo);
}
Serial.println(temp);
threshold = int(temp/500);

Serial.print("Ambient light level set at ");
Serial.println(threshold);

  
}
```