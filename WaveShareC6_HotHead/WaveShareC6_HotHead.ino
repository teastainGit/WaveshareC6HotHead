//https://github.com/Cincinnatu/TFT_eSPI
//https://github.com/Bodmer/TFT_eSPI/pull/3399#issuecomment-2562186046
//Magnus's User_Setup.h, except the first
//#define, for which I use #define USER_SETUP_INFO "User_Setup".
#include <TFT_eSPI.h>
#include "HotHead170.h"
#include <Adafruit_NeoPixel.h>
unsigned colour = 0xFFFF;
TFT_eSPI tft = TFT_eSPI();
#define topbutton 9
#define PIN_LCD_BL 22  // BackLight enable pin (see Dimming.txt)

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define RGBpin 8
#define NUMPIXELS 1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RGBpin, NEO_GRB + NEO_KHZ800);

int progress = 0;
int x;
int blocks = 0;
int i = 0;
bool ticktock = true;

void setup() {
  pinMode(PIN_LCD_BL, OUTPUT);  // BackLight enable pin
  pinMode(topbutton, INPUT);    //Left button  pulled up, push = 0
  pixels.begin();               //Init the NeoPixel library.
  delay(100);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green, Red, Blue
  pixels.show();
  digitalWrite(PIN_LCD_BL, HIGH);
  Serial.begin(115200);  // be sure to set USB CDC On Boot: "Enabled"
  delay(500);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);  //horiz / vert<> position/dimension
  tft.pushImage(170, 0, 146, 170, HotHead170);
  tft.setTextSize(1);
  tft.setTextDatum(4);

  delay(100);
  Serial.println("In setup!");

  delay(2000);
}

void loop() {
  ticktock = !ticktock;
  if (ticktock) {
    Serial.println("tick");
  } else {
    Serial.println("tock");
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 40);
  if (!digitalRead(topbutton)) {
    tft.println("Pushed!");
    pixels.setPixelColor(0, pixels.Color(10, 0, 0));  // Green, Red, Blue
    pixels.show();
  } else {
    tft.println("        ");
    tft.fillRect(5, 42, 145, 20, BLACK);
    pixels.setPixelColor(0, pixels.Color(0, 0, 10));  // Green, Red, Blue
    pixels.show();
  }
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillRect(20, 71, 60, 25, TFT_BLACK);
  tft.drawString(String(progress) + "% ", 50, 85, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(" Demo ", 115, 85, 4);
  delay(100);
  tft.drawRoundRect(25, 110, 115, 18, 3, TFT_GREEN);  //frame
  progress++;
  if (progress >= 100) {
    progress = 0;
    tft.fillRect(28, 114, 106, 10, TFT_BLACK);  //resets the rectangle line fill
  }

  if (progress >= 0 && progress < 37) {
    colour = 0x001F;  //BLUE
  }
  if (progress > 37 && progress < 72) {
    colour = 0x07E0;  //GREEN
  }
  if (progress > 72 && progress <= 100) {
    colour = 0xF800;  //RED
  }

  tft.fillRect(progress + 29, 114, 1, 10, colour);  // draws the lines


  tft.drawRect(5, 7, 135, 22, TFT_BLUE);     //"Top"  text box
  tft.drawRect(10, 146, 110, 22, TFT_BLUE);  // "Bottom" text box
  if (digitalRead(9)) {                      //  normally open sw, pulled up.
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  }

  tft.drawString("^ Top GPIO 9 'Boot'", 70, 18, 2);
  tft.drawString("v Bottom RST", 60, 157, 2);
}
