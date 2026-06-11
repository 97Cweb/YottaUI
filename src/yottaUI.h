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


enum YottaTextMode {
  YOTTA_TEXT_WRAP,
  YOTTA_TEXT_NO_WRAP
};

class YottaTextBox {
public:
  YottaTextBox(YottaScreen* screen,
               int16_t x, int16_t y, int16_t w, int16_t h,
               YottaTextMode mode = YOTTA_TEXT_WRAP)
    : _screen(screen), _x(x), _y(y), _w(w), _h(h), _mode(mode) {}

  void setText(const char* text) {
    _text = text;
    _scrollX = 0;
    _scrollY = 0;
  }

  void setMode(YottaTextMode mode) {
    _mode = mode;
    _scrollX = 0;
    _scrollY = 0;
  }

  void scrollUp()    { if (_scrollY > 0) _scrollY--; }
  void scrollDown()  { _scrollY++; }
  void scrollLeft()  { if (_scrollX > 0) _scrollX--; }
  void scrollRight() { if (_mode == YOTTA_TEXT_NO_WRAP) _scrollX++; }

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
        if (_mode == YOTTA_TEXT_NO_WRAP) {
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
  int start = 0;
  int len = line.length();

  while (start < len) {
    // Do not start a visual line with spaces
    while (start < len && line[start] == ' ') {
      start++;
    }

    if (start >= len) break;

    int end = start + cols;

    if (end >= len) {
      end = len;
    } else {
      // Try to wrap at the last space within the allowed width
      int wrapAt = -1;

      for (int i = end; i > start; i--) {
        if (line[i] == ' ') {
          wrapAt = i;
          break;
        }
      }

      if (wrapAt > start) {
        end = wrapAt;
      }
      // else: no space found, so split the long word at cols
    }

    String chunk = line.substring(start, end);

    if (lineIndex >= _scrollY && visibleLine < rows) {
      _screen->text(_x + 2,
                    _y + 2 + visibleLine * (8 * textSize),
                    chunk.c_str(),
                    fg,
                    textSize);
      visibleLine++;
    }

    lineIndex++;

    start = end;

    // Skip the space that caused wrapping
    while (start < len && line[start] == ' ') {
      start++;
    }
  }

  if (len == 0) {
    lineIndex++;
  }
}

  YottaScreen* _screen;
  int16_t _x, _y, _w, _h;
  YottaTextMode _mode;
  String _text;
  int16_t _scrollX = 0;
  int16_t _scrollY = 0;
};
