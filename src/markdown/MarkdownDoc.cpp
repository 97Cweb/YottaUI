#pragma once 
#include "MarkdownDoc.h"

void MarkdownDoc::parse(const String& markdown) {
  _blocks.clear();

  int start = 0;

  while (start < markdown.length()) {
    int end = markdown.indexOf('\n', start);
    if (end == -1) end = markdown.length();

    String line = markdown.substring(start, end);
    line.trim();

    if (line.length() > 0) {
      MdBlock block;
      MdSpan span;

      span.type = SPAN_TEXT;
      span.target = "";
      
      int level = 0;
      while(line[level] == '#'){     
        level++;
      }

      if (level > 0 && level <= 5 && line[level] == ' ') {
        block.type = MD_HEADING;
        block.level = level;

        span.text = line.substring(level + 1);
      } 
      else {
        block.type = MD_PARAGRAPH;
        span.text = line;
      }

      block.spans.push_back(span);
      _blocks.push_back(block);
    }

    start = end + 1;
  }
}
