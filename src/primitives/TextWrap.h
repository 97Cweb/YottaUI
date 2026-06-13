#pragma once

#include <Arduino.h>

class TextWrap {
public:
  static String nextLine(const String& text, int& start, int16_t cols) {
    // Skip leading spaces so no line starts with a space
    while (start < text.length() && text[start] == ' ') {
      start++;
    }

    if (start >= text.length()) {
      return "";
    }

    int remaining = text.length() - start;

    // Fits on one line
    if (remaining <= cols) {
      String line = text.substring(start);
      start = text.length();
      return line;
    }

    int end = start + cols;

    // Prefer breaking at newline if one appears first
    for (int i = start; i < end; i++) {
      if (text[i] == '\n') {
        String line = text.substring(start, i);
        start = i + 1;
        return line;
      }
    }

    // Try to break at the last space before cols
    int breakAt = -1;
    for (int i = end; i > start; i--) {
      if (text[i] == ' ') {
        breakAt = i;
        break;
      }
    }

    if (breakAt > start) {
      String line = text.substring(start, breakAt);
      start = breakAt + 1;
      return line;
    }

    // No good space found, hard-break the long word
    String line = text.substring(start, end);
    start = end;
    return line;
  }

  static int countWrappedLines(const String& text, int16_t cols) {
    int count = 0;
    int start = 0;

    while (start < text.length()) {
      nextLine(text, start, cols);
      count++;
    }

    if (text.length() == 0) {
      count = 1;
    }

    return count;
  }
};
