#ifndef YOTTA_TEXTBOX_H
#define YOTTA_TEXTBOX_H

#include <Arduino.h>
#include "TextWrap.h"

class YottaScreen;

enum TextMode {
  TEXT_WRAP,
  TEXT_NO_WRAP
};

class TextBox {
public:
  TextBox(YottaScreen* screen,
               int16_t x, int16_t y, int16_t w, int16_t h,
               TextMode mode = TEXT_WRAP)
    : _screen(screen), _x(x), _y(y), _w(w), _h(h), _mode(mode) {}

  void setText(const char* text) {
    _text = text;
    _scrollX = 0;
    _scrollY = 0;
  }

  void setMode(TextMode mode) {
    _mode = mode;
    _scrollX = 0;
    _scrollY = 0;
  }

  void scrollUp()    { if (_scrollY > 0) _scrollY--; }
  void scrollDown()  { _scrollY++; }
  void scrollLeft()  { if (_scrollX > 0) _scrollX--; }
  void scrollRight() { if (_mode == TEXT_NO_WRAP) _scrollX++; }

  void draw(uint16_t bg = 0x0000,
            uint16_t fg = 0xFFFF,
            uint16_t border = 0xFFFF,
            uint8_t textSize = 1) {
    _screen->fillRect(_x, _y, _w, _h, bg);
    _screen->rect(_x, _y, _w, _h, border);

    int16_t charW = 6 * textSize;
    int16_t charH = 8 * textSize;
    int16_t cols = (_w - 4) / charW;
    int16_t rows = (_h - 4) / charH;

    int16_t lineIndex = 0;
    int16_t visibleLine = 0;

    String line = "";

    for (uint16_t i = 0; i <= _text.length(); i++) {
      char c = (i < _text.length()) ? _text[i] : '\n';

      if (c == '\n') {
        if (_mode == TEXT_NO_WRAP) {
          drawLineIfVisible(line, lineIndex, visibleLine, rows, cols, fg, textSize);
          lineIndex++;
        } else {
          drawWrappedLine(line, lineIndex, visibleLine, rows, cols, fg, textSize);
        }
        line = "";
      } else {
        line += c;
      }
    }
  }

private:
  void drawLineIfVisible(String line,
                         int16_t lineIndex,
                         int16_t& visibleLine,
                         int16_t rows,
                         int16_t cols,
                         uint16_t fg,
                         uint8_t textSize) {
    if (lineIndex < _scrollY) return;
    if (visibleLine >= rows) return;

    String shown = line.substring(_scrollX, _scrollX + cols);

    _screen->text(_x + 2,
                  _y + 2 + visibleLine * (8 * textSize),
                  shown.c_str(),
                  fg,
                  textSize);

    visibleLine++;
  }

  void drawWrappedLine(String line,
                       int16_t& lineIndex,
                       int16_t& visibleLine,
                       int16_t rows,
                       int16_t cols,
                       uint16_t fg,
                       uint8_t textSize) {
    if (line.length() == 0) {
      lineIndex++;
      return;
    }

    int start = 0;

    while (start < line.length()) {
      String chunk = TextWrap::nextLine(line, start, cols);

      if (chunk.length() == 0 && start >= line.length()) {
        break;
      }

      if (lineIndex >= _scrollY && visibleLine < rows) {
        _screen->text(_x + 2,
                      _y + 2 + visibleLine * (8 * textSize),
                      chunk.c_str(),
                      fg,
                      textSize);
        visibleLine++;
      }

      lineIndex++;
    }
  }

  YottaScreen* _screen;
  int16_t _x, _y, _w, _h;
  TextMode _mode;
  String _text;
  int16_t _scrollX = 0;
  int16_t _scrollY = 0;
};

#endif
