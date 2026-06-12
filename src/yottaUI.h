#ifndef YOTTA_UI_H
#define YOTTA_UI_H

#include <Arduino.h>
#include <Adafruit_GFX.h>


class YottaScreen {
public:
  YottaScreen(Adafruit_GFX* display)
    : _display(display) {}

  void setRotation(int8_t rotation) {
    _display->setRotation(rotation);
    _w = _display->width();
    _h = _display->height();
  }

  void clear(uint16_t color = 0x0000) {
    _display->fillScreen(color);
  }

  void text(int16_t x, int16_t y, const char* msg,
            uint16_t color = 0xFFFF, uint8_t size = 1) {
    _display->setCursor(x, y);
    _display->setTextColor(color);
    _display->setTextSize(size);
    _display->print(msg);
  }

  void rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _display->drawRect(x, y, w, h, color);
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _display->fillRect(x, y, w, h, color);
  }

  int16_t width() const { return _w; }
  int16_t height() const { return _h; }

private:
  Adafruit_GFX* _display;
  int16_t _w = 0;
  int16_t _h = 0;
};


#include "primitives/textbox.h"
#include "markdown/MarkdownDoc.h"
#include "markdown/MarkdownViewer.h"

#endif
