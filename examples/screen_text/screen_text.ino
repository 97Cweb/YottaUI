#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Fonts/FreeMono9pt7b.h>

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
Adafruit_ILI9341  lower(LOWER_CS, LOWER_DC, LOWER_RST);

inline void selectUpper()   { digitalWrite(LOWER_CS, HIGH); digitalWrite(UPPER_CS, LOW); }

void setup() {
  Serial.begin(115200);
  delay(200);



  SPI.begin(TFT_SCK, -1, TFT_MOSI);

  // --- INIT UPPER ---
  selectUpper();
  upper.begin();
  upper.setSPISpeed(8000000);
  upper.setRotation(3);

  delay(1000);
  upper.fillScreen(ILI9341_BLACK);
  delay(1000);
  upper.setFont(&FreeMono9pt7b);
  upper.setTextWrap(true);
  upper.setTextColor(ILI9341_BLUE,ILI9341_WHITE);
  upper.setTextSize(2);
  upper.setCursor(10, 10);
  const String text = "Bananas are good to eat on tuesdays at eight thirty";
  upper.print(text);

}

void loop() {}