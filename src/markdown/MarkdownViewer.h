#pragma once

#include <Arduino.h>
#include <vector>
#include "MarkdownDoc.h"
#include "../primitives/TextWrap.h"
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>

enum DrawItemType {
  DRAW_TEXT,
  DRAW_LINK_TEXT,
  DRAW_IMAGE_BOX,
  DRAW_TABLE_BOX,
  DRAW_SIDEBAR_ITEM
};

struct DrawItem {
  DrawItemType type;
  SpanType spanType;
  int16_t x, y, w, h;
  String text;
  String target;
  bool focusable;
  uint8_t textSize;
  const GFXfont* font;
};

class MarkdownViewer {
public:
  MarkdownViewer(YottaScreen* screen,
                 int16_t x,
                 int16_t y,
                 int16_t w,
                 int16_t h)
    : _screen(screen),
      _x(x),
      _y(y),
      _w(w),
      _h(h) {}

  void setDocument(MarkdownDoc* doc) {
    _doc = doc;
    _scrollY = 0;
    _focusedItem = -1;
    _focus = false;
    buildLayout();
  }

  void draw() {
    if (!_screen) return;

    _screen->fillRect(_x, _y, _w, _h, _bg);
    _screen->rect(_x, _y, _w, _h, _fg);

    if (!_doc) return;

    for (const DrawItem& item : _items) {
      DrawItem visible = item;

      visible.y = item.y - _scrollY;

      if (visible.y + visible.h < _y) continue;
      if (visible.y > _y + _h) continue;

      drawItem(visible);
    }
  }

  void scroll(int pixels) {
    _scrollY += pixels;

    if (_scrollY < 0) {
      _scrollY = 0;
    }

    int16_t contentBottom = _y;

    if (_items.size() > 0) {
      const DrawItem& last = _items[_items.size() - 1];
      contentBottom = last.y + last.h;
    }

    int16_t visibleBottom = _y + _h;

    int maxScroll = contentBottom - visibleBottom;

    if (maxScroll < 0) {
      maxScroll = 0;
    }

    if (_scrollY > maxScroll) {
      _scrollY = maxScroll;
    }
  }

  void focusNext() {
    // Later: move through focusable DrawItems
  }

  void toggleFocus() {
    _focus = !_focus;
  }

private:
  void buildLayout() {
  _items.clear();

  if (!_doc) return;

  const GFXfont* normalFont = &FreeMono9pt7b;
  const GFXfont* boldFont   = &FreeMonoBold9pt7b;

  const GFXfont* currentFont = normalFont;

  int16_t lineY = _y + 2;

  for (const MdBlock& block : _doc->getBlocks()) {
    uint8_t itemTextSize = 1;

    if (block.type == MD_HEADING) {
      if (block.level == 1) itemTextSize = 3;
      else if (block.level > 1) itemTextSize = 2;
      else itemTextSize = 1;
    }

    

    int16_t xCursor = _x + 2;

    int16_t charH = _screen->textHeight("Mg", itemTextSize, normalFont);
    int16_t lineH = charH + 4;
    int16_t baseline = _screen->textBaselineOffset("Mg", itemTextSize, normalFont);
    int16_t charW = _screen->textWidth("M", itemTextSize, currentFont);
    int16_t colsLeft = (_x + _w - 4 - xCursor) / charW;
    
    

    for (const MdSpan& span : block.spans) {
      if (span.type == SPAN_BOLD) {
        currentFont = boldFont;
      } else {
        currentFont = normalFont;
      }

      int start = 0;

      while (start < span.text.length()) {
        String remaining = span.text.substring(start);

        if (colsLeft <= 0) {
          xCursor = _x + 2;
          lineY += lineH;
          colsLeft = (_w - 4) / charW;
        }

        int oldStart = start;
        bool trimLeading = (xCursor == _x + 2);
        String piece = TextWrap::nextLine(span.text, start, colsLeft,trimLeading);

        if (piece.length() == 0 && start == oldStart) {
          break;
        }

        DrawItem item;
        item.type = DRAW_TEXT;
        item.spanType = span.type;
        item.x = xCursor;
        item.y = lineY + baseline;
        item.w = piece.length() * charW;
        item.h = lineH;
        item.text = piece;
        item.target = span.target;
        item.focusable = false;
        item.textSize = itemTextSize;
        item.font = currentFont;

        _items.push_back(item);

        xCursor += item.w;

        // If TextWrap consumed to a wrap boundary, move to next line
        if (start < span.text.length()) {
          xCursor = _x + 2;
          lineY += lineH;
        }
      }
    }

    // block spacing
    lineY += lineH;
  }
}

  void drawItem(const DrawItem& item) {
    if (item.type == DRAW_TEXT) {
        Serial.println(item.text);
        Serial.println(item.y);
        _screen->text(item.x, item.y, item.text.c_str(), _fg, _bg, item.textSize, item.font);
    }
  }

  YottaScreen* _screen;

  int16_t _x;
  int16_t _y;
  int16_t _w;
  int16_t _h;
  uint16_t _bg = 0x0000;
  uint16_t _fg = 0xFFFF;

  MarkdownDoc* _doc = nullptr;
  std::vector<DrawItem> _items;

  int16_t _scrollY = 0;
  int16_t _focusedItem = -1;
  bool _focus = false;
};
