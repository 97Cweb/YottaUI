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


const String testMarkdown = R"md(
# Grief and Pills: Part 1 - The Holy Saturday of Our Age

2025-06-11 22:48:13

grief.png

[Part 1](https://yottarock.ca/blog/55) | [Part 2](https://yottarock.ca/blog/56) | [Part 3](https://yottarock.ca/blog/57) | [Part 4](https://yottarock.ca/blog/58) | [Part 5](https://yottarock.ca/blog/59) | [Part 6](https://yottarock.ca/blog/60) | [Part 7](https://yottarock.ca/blog/61) | [Part 8](https://yottarock.ca/blog/62)

This is **part 1** for a study on current society. Given that you found my little corner of the internet, you must have been wading for a while, and have heard of the terms of multiple coloured pills: Red pills, blue pills, and even black pills, and possibly more. I am here to argue that these pills and their main philosophy standpoint are all coping strategies of grief. Not on a personal level of grief, but on a civilizational level. 
)md";

Adafruit_ILI9341 upper(UPPER_CS, UPPER_DC, UPPER_RST);
YottaScreen upperScreen(&upper);
MarkdownViewer mdView(&upperScreen,0,0,320,240);
MarkdownDoc mdDoc(testMarkdown);

inline void selectUpper()   { digitalWrite(LOWER_CS, HIGH); digitalWrite(UPPER_CS, LOW); }


void setup() {
  Serial.begin(115200);
  delay(200);


  SPI.begin(TFT_SCK, -1, TFT_MOSI);
  upper.begin();
  // --- INIT UPPER ---
  selectUpper();
  upper.setSPISpeed(2000000);
  upperScreen.setRotation(3);

  delay(1000);


  mdView.setDocument(&mdDoc);
  mdView.scroll(400);
  mdView.draw();
  

}

void loop() {
  
}


