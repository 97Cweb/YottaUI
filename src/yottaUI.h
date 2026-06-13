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
            uint16_t fg = 0xFFFF, uint16_t bg = 0x0000, uint8_t size = 1, const GFXfont* font = NULL) {
    _display->setFont(font);    
    _display->setCursor(x, y);
    _display->setTextColor(fg,bg);
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

  uint16_t textWidth(const char* msg,
                     uint8_t size = 1,
                     const GFXfont* font = NULL) {
    int16_t x1, y1;
    uint16_t w, h;

    _display->setFont(font);
    _display->setTextSize(size);
    _display->getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);

    return w;
  }

  uint16_t textHeight(const char* msg,
                      uint8_t size = 1,
                      const GFXfont* font = NULL) {
    int16_t x1, y1;
    uint16_t w, h;

    _display->setFont(font);
    _display->setTextSize(size);
    _display->getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);

    return h;
  }

  int16_t textBaselineOffset(const char* msg,
                             uint8_t size = 1,
                             const GFXfont* font = NULL) {
    int16_t x1, y1;
    uint16_t w, h;

    _display->setFont(font);
    _display->setTextSize(size);
    _display->getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);

    return -y1;
  }


private:
  Adafruit_GFX* _display;
  int16_t _w = 0;
  int16_t _h = 0;
};


#include "primitives/TextBox.h"
#include "markdown/MarkdownDoc.h"
#include "markdown/MarkdownViewer.h"

#endif
