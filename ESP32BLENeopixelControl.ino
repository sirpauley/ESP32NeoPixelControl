/*Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
 * Thanks to Neil Kolbans for his efoorts in adding the support to Arduino IDE
 * Turotial on: www.circuitdigest.com 
 */

#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino
#include "esp_bt_device.h"
#include "FastLED.h"

#include <ArduinoJson.h>

#define LED_TYPE SK9822
#define COLOR_ORDER RGB

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            12
#define PIN2           13
#define CLOCK_PIN      16

//control neoPixel through bluetooth

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

// Define the array of leds
CRGB leds[NUMPIXELS];
CRGB leds2[NUMPIXELS];

uint8_t redMaster = 0;
uint8_t greenMaster = 0;
uint8_t blueMaster = 0;

int j = 0;

BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
int LED_BUILTIN = 33;
int FLASH_LAMP =  4;
String modeSetting = "";

void setup() {
  Serial.begin(115200); //Start Serial monitor in 115200

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
  FastLED.addLeds<NEOPIXEL, PIN2>(leds2, NUMPIXELS);
  
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  printDeviceAddress();

  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
  pinMode(FLASH_LAMP, OUTPUT);//Specify the LAMLIGHT output
  
}

//ESP_BT
void printDeviceAddress(){
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++){
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
    if(i < 5){
      Serial.print(":");
      }
    }
  }

void led_white_on()
{
  for(int i=0;i<NUMPIXELS;i++){
    leds[i] = CRGB::White;
  }
  delay(5);
  FastLED.show();

  Serial.println("STRIP turned ON");
}

void led_white_off()
{
  for(int i=0;i<NUMPIXELS;i++){
    leds[i] = CRGB::Black;
  }
  delay(5);
  FastLED.show();
//  Serial.println("STRIP turned OFF");
}

void setColors( int RED , int GREEN , int BLUE )
{

  for(int i=0;i<NUMPIXELS;i++){
    leds[i].setRGB(RED, GREEN, BLUE);
    
    int red2 = 255 - RED;
    int green2 = 255 - GREEN;
    int blue2 = 255 - BLUE;
    leds2[i].setRGB(red2, green2, blue2);
  }
  FastLED.show();

  Serial.println("Colors set to");
  Serial.print("RED - ");Serial.println(RED);
  Serial.print("GREEN - ");Serial.println(GREEN);
  Serial.print("BLUE - ");Serial.println(BLUE);
}

//modes start here

void showStrip() {
   FastLED.show();
}

void setPixel(int Pixel, int red, int green, int blue) {
  leds[Pixel].setRGB(red, green, blue);
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    leds[i].setRGB(red, green, blue);
  }
  FastLED.show();
}

void RGBLoop(){
  for(int j = 0; j < 3; j++ ) {
    // Fade IN
    for(int k = 0; k < 256; k++) {
      switch(j) {
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) {
      switch(j) {
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
  }
}

void FadeInOut(int red, int green, int blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void Strobe(int red, int green, int blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void HalloweenEyes(int red, int green, int blue,
                   int EyeWidth, int EyeSpace,
                   boolean Fade, int Steps, int FadeDelay,
                   int EndPause){
  randomSeed(analogRead(0));
 
  int i;
  int StartPoint  = random( 0, NUMPIXELS - (2*EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;
 
  for(i = 0; i < EyeWidth; i++) {
    setPixel(StartPoint + i, red, green, blue);
    setPixel(Start2ndEye + i, red, green, blue);
  }
 
  showStrip();
 
  if(Fade==true) {
    float r, g, b;
 
    for(int j = Steps; j >= 0; j--) {
      r = j*(red/Steps);
      g = j*(green/Steps);
      b = j*(blue/Steps);
     
      for(i = 0; i < EyeWidth; i++) {
        setPixel(StartPoint + i, r, g, b);
        setPixel(Start2ndEye + i, r, g, b);
      }
     
      showStrip();
      delay(FadeDelay);
    }
  }
 
  setAll(0,0,0); // Set all black
 
  delay(EndPause);
}

void CylonBounce(int red, int green, int blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUMPIXELS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i+j, red, green, blue);
    for(int j = 1; j <= EyeSize; j++) {
      leds[i+j].setRGB(red, green, blue);
    }
    setPixel(i+j, red, green, blue);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUMPIXELS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i+j, red, green, blue);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+j, red, green, blue);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

//
//well KITT starts here
void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void CenterToOutside(int red, int green, int blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i =((NUMPIXELS-EyeSize)/2); i>=0; i--) {
    setAll(0,0,0);
   
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
   
    setPixel(NUMPIXELS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUMPIXELS-i-j, red, green, blue);
    }
    setPixel(NUMPIXELS-i-EyeSize-1, red/10, green/10, blue/10);
   
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i<=((NUMPIXELS-EyeSize)/2); i++) {
    setAll(0,0,0);
   
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
   
    setPixel(NUMPIXELS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUMPIXELS-i-j, red, green, blue);
    }
    setPixel(NUMPIXELS-i-EyeSize-1, red/10, green/10, blue/10);
   
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUMPIXELS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUMPIXELS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

//ends here
//

void Twinkle(int red, int green, int blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     setPixel(random(NUMPIXELS),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}

void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     setPixel(random(NUMPIXELS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}

void RunningLights(int red, int green, int blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUMPIXELS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUMPIXELS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}

//modes END here

void colorMode(int red, int green, int blue, String modeSetting){
  while(true){
    if(modeSetting == "FadeInOut")
      FadeInOut(red, green,blue);
    }
  }

void loop() {

    // Allocate the JSON document
    StaticJsonDocument<200> jsonBuffer;
    
    while (ESP_BT.available () > 0){
      Serial.println("Input Recieved.");
      
//      ESP_BT.readBytesUntil('\n', buffer, 9);
//      int incomingValue = atoi(buffer);

      String rgbCommand = ESP_BT.readString();

      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(3) + 20;
      DynamicJsonDocument RGB(capacity);
      
      String json = rgbCommand;
//      const char* json = "{\"red\":255,\"green\":255,\"blue\":255}";
      
      deserializeJson(RGB, json);
      
      int red = RGB[0]["red"]; // 255
      int green = RGB[0]["green"]; // 255
      int blue = RGB[0]["blue"]; // 255

      Serial.println("input recieved:");
      Serial.print("RED: ");Serial.println(red);
      Serial.print("GREEN: ");Serial.println(green);
      Serial.print("BLUE: ");Serial.println(blue);

//      setColors(red,green,blue);
//        colorMode(red,green,blue, "FadeInOut");
        redMaster = red;
        greenMaster = green;
        blueMaster = blue;
        modeSetting = "FadeInOut";
      
      Serial.println(rgbCommand);
      ESP_BT.println(rgbCommand);
      }

//modeSetting can be == to the following
//RGBLoop
//FadeInOut
//Strobe
//CylonBounce
//NewKITT
//Twinkle
//TwinkleRandom
//RunningLights
//solid

      if(modeSetting == "RGBLoop" ){
         //fades in and out R G B
          RGBLoop();
       }

      if(modeSetting == "FadeInOut" ){
          FadeInOut(redMaster, greenMaster, blueMaster);
       }

      if(modeSetting == "Strobe" ){
           // Fast strobe with custom timers and colors:
          //Strobe(red, green, blue, StrobeCount, FlashDelay, EndPause){
          Strobe(redMaster, greenMaster, blueMaster, 10, 50, 1000);
       }

      if(modeSetting == "CylonBounce" ){
          //like from left to right
          //with rgb
          //CylonBounce(int red, int green, int blue, int EyeSize, int SpeedDelay, int ReturnDelay)
//           CylonBounce(redMaster, greenMaster, blueMaster, 4, 10, 50);
//           CylonBounce(redMaster, greenMaster, blueMaster, 2, 10, 50);
           CylonBounce(redMaster, greenMaster, blueMaster, 1, 10, 50);
       }

      if(modeSetting == "NewKITT" ){
          //like night riders car but wwith RGB
          //NewKITT(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)
//          NewKITT(redMaster, greenMaster, blueMaster, 8, 10, 50);
          NewKITT(redMaster, greenMaster, blueMaster, 1, 10, 50); //for Dev use
       }

      if(modeSetting == "Twinkle" ){
         //twinkels looks like rain
         //Twinkle(int red, int green, int blue, int Count, int SpeedDelay, boolean OnlyOne)
          Twinkle(redMaster, greenMaster, blueMaster, 10, 100, false);
       }

      if(modeSetting == "TwinkleRandom" ){
          // same as twwinkel but with randdom colors
          //TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne)
          TwinkleRandom(20, 100, false);
       }

      if(modeSetting == "RunningLights" ){
          // running lights
          //RunningLights(R, G, B, int WaveDelay);
          RunningLights(redMaster, greenMaster, blueMaster, 50);
       }

      if( modeSetting == "solid" || modeSetting == "" ){
          //sets all LEDs to one color
          //use for solidd mode
          setAll(redMaster, greenMaster, blueMaster);
          showStrip();
       }
             
}
