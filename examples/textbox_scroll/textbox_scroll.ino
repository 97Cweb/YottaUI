#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <yottaUI.h>

// Proven working SPI + ILI9341 pins
#define TFT_SCK  12
#define TFT_MOSI 11
// Upper (ILI9341) pins - proven
#define UPPER_CS   10
#define UPPER_DC   9
#define UPPER_RST  6

#define LOWER_CS  14
#define LOWER_DC  8
#define LOWER_RST 5


Adafruit_ILI9341 upper(UPPER_CS, UPPER_DC, UPPER_RST);
YottaScreen upperScreen(&upper);
TextBox box(&upperScreen, 10, 40,300,180, TEXT_WRAP);

inline void selectUpper()   { digitalWrite(LOWER_CS, HIGH); digitalWrite(UPPER_CS, LOW); }


void setup() {
  Serial.begin(115200);
  delay(200);


  SPI.begin(TFT_SCK, -1, TFT_MOSI);
  upper.begin();
  // --- INIT UPPER ---
  selectUpper();
  upper.setSPISpeed(8000000);
  upperScreen.setRotation(3);

  upperScreen.clear(ILI9341_BLACK);
  
  box.setText(
    "This is a wrapped textbox. It should wrap long lines automatically.\n"
    "Second paragraph here.\n"
    "Later we can use buttons to scroll up and down."
  );

  box.draw(ILI9341_BLACK, ILI9341_WHITE, ILI9341_CYAN,2);

  // box.setMode(YOTTA_TEXT_NO_WRAP);
  // box.setText("This line is intentionally very very very very very very long.");
  // box.scrollRight();
  // box.draw();

}

void loop() {}


