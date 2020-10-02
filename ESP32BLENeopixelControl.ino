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

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

int j = 0;

BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
int LED_BUILTIN = 33;
int FLASH_LAMP =  4;

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

void colorDemoStupid(){
  // Turn the LED on, then pause
    for(int i=0;i<NUMPIXELS;i++){
      leds[i] = CRGB::Blue;
      leds2[i] = CRGB::White;
    }
    FastLED.show();
  delay(500);
  for(int i=0;i<NUMPIXELS;i++){
      leds[i] = CRGB::Red;
    leds2[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
  for(int i=0;i<NUMPIXELS;i++){
    leds[i] = CRGB::Blue;
    leds2[i] = CRGB::White;
  }
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  for(int i=0;i<NUMPIXELS;i++){
    leds[i] = CRGB::Black;
    leds2[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
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

      setColors(red,green,blue);
      
      Serial.println(rgbCommand);
      ESP_BT.println(rgbCommand);
      }
    
  
}
