# ESP32NeoPixelControl
This is the Arduino library I created, a very basic script for controlling the NeoPixels through Bluetooth.

Also, note I have created a Mobile app with Cordova to send commands to the ESP32 the repo is called [neoPixelBT](https://github.com/sirpauley/neoPixelBT).

I wanted to control NeoPixel LEDs from a mobile app, This code takes instructions through Bluetooth as a JSON object which I will explain.

## Install the following libraries
### 1. Add ESP32 to arduino IDE [link](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
* Add ```https://dl.espressif.com/dl/package_esp32_index.json``` into the “Additional Board Manager URLs”
* Go to Tools > Board > Boards Manager
* Install "ESP32 by Espressif Systems"
### 2. Installing libraries
* Go to "Sketch > Included library > Manage libraries.."
* Search for ```FastLED.h```
* Click on install
### 3. JSON library
* Go to "Sketch > Included library > Manage libraries.."
* Search for ```ArduinoJson.h```
* Click on install
### 4. Magic
Everything should be working at this point, I will list the libraries I used
- "BluetoothSerial.h"
- "esp_bt_device.h"
- "FastLED.h"
- <ArduinoJson.h>

## How to use
Well it is very simple, you need to create a JSON object as follow
```
{
    "red":255,
    "green":255,
    "blue":255,
    "mode":"solid",
}
```
Then send this object to the ESP32, The mode "solid" just means it will light up the LEDs according to the RGB color you gave it, I will explain this in more detail.

## Modes
Here is a list of modes:
* RGBLoop
* FadeInOut
* Strobe
* CylonBounce
* NewKITT
* Twinkle
* TwinkleRandom
* RunningLights
* solid

### RGBLoop
This mode doesn't use the RGB you send it
![alt text](gif/RGBLoop.gif?raw=true "RGBLoop")

### FadeInOut
![alt text](gif/FadeInAndOut.gif?raw=true "RGBLoop")

### Strobe
![alt text](gif/Strobe.gif?raw=true "RGBLoop")

### CylonBounce
![alt text](gif/CylonBounce.gif?raw=true "RGBLoop")

### NewKITT
If you need to ask who KITT is, then you are too young to know
![alt text](gif/NewKITT.gif?raw=true "RGBLoop")

### Twinkle
![alt text](gif/Twinkle.gif?raw=true "RGBLoop")

### TwinkleRandom
This mode doesn't use the RGB you send it, it uses random colors
![alt text](gif/RandomColorTwinkle.gif?raw=true "RGBLoop")

### RunningLights
![alt text](gif/RunningLights.gif?raw=true "RGBLoop")

Videos and code for the modes found on [tweaking4all](https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/)

## Note
* Well keep in mind I build this project for installing neoPixel on my Bike.
* I had to use FastLed because I want to send data to 2 different pins because I have the NeoPixel LEDs installed  on both sides.
* So basically the 2 sides mirror each other according to what mode I selected

## Reference to tools that helped me
* Arduino JSON calculator for size [arduino JSON guide](https://arduinojson.org/v6/assistant/)
* ALgorithmes for color modes, Thanks [tweaking4all](https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects)
* Decoding and encoding JSON object with Arduino [Link](https://randomnerdtutorials.com/decoding-and-encoding-json-with-arduino-or-esp8266/)
* This app helped me to play around with the ESP and mock out some of my initial commands [Bluetooth Terminal](https://play.google.com/store/apps/details?id=Qwerty.BluetoothTerminal&hl=en_ZA)
* This is the link to my app repo [Link](https://github.com/sirpauley/neoPixelBT)