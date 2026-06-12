#pragma once

#include <Arduino.h>
#include <vector>
#include "MarkdownDoc.h"
#include "../primitives/textbox.h"

enum DrawItemType {
  DRAW_TEXT,
  DRAW_LINK_TEXT,
  DRAW_IMAGE_BOX,
  DRAW_TABLE_BOX,
  DRAW_SIDEBAR_ITEM
};

struct DrawItem {
  DrawItemType type;
  int16_t x, y, w, h;
  String text;
  String target;
  bool focusable;
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
      _h(h),
      _textbox(screen, x, y, w, h) {}

  void setDocument(MarkdownDoc* doc) {
    _doc = doc;
    _scrollY = 0;
    _focusedItem = -1;
    _focus = false;
    buildLayout();
  }

  void draw() {
    if (!_doc) return;

    String text;

    for (const MdBlock& block : _doc->getBlocks()) {
      if (block.type == MD_PARAGRAPH) {
        for (const MdSpan& span : block.spans) {
          text += span.text;
        }
        text += "\n";
      }
    }

    _textbox.setText(text.c_str());
    _textbox.setMode(TEXT_WRAP);
    _textbox.draw();
  }

  void scroll(int lines) {
    _scrollY += lines;
    if (_scrollY < 0) _scrollY = 0;
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

    // First pass: no real layout yet.
    // Later this creates DrawItems for text, links, images, tables.
  }

  void drawItem(const DrawItem& item) {
    // Later.
  }

  YottaScreen* _screen;

  int16_t _x;
  int16_t _y;
  int16_t _w;
  int16_t _h;

  MarkdownDoc* _doc = nullptr;
  std::vector<DrawItem> _items;

  int16_t _scrollY = 0;
  int16_t _focusedItem = -1;
  bool _focus = false;

  TextBox _textbox;
};
