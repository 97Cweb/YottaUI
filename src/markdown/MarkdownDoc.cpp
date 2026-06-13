#pragma once 
#include "MarkdownDoc.h"

void MarkdownDoc::parse(const String& markdown) {
  _blocks.clear();

  String currentBlock = "";
  int start = 0;

  while (start <= markdown.length()) {
    int end = markdown.indexOf('\n', start);
    if (end == -1) end = markdown.length();

    String line = markdown.substring(start, end);
    line.trim();

    bool blankLine = (line.length() == 0);

    if (blankLine || start == markdown.length()) {
      if (currentBlock.length() > 0) {
        MdBlock block;

        int level = 0;
        while (level < currentBlock.length() && currentBlock[level] == '#') {
          level++;
        }

        if (level > 0 &&
            level <= 5 &&
            level < currentBlock.length() &&
            currentBlock[level] == ' ') {
          block.type = MD_HEADING;
          block.level = level;

          String headingText = currentBlock.substring(level + 1);
          parseSpans(headingText, block);
        } else {
          block.type = MD_PARAGRAPH;
          block.level = 0;

          parseSpans(currentBlock, block);
        }

        _blocks.push_back(block);
        currentBlock = "";
      }
    } else {
      if (currentBlock.length() > 0) {
        currentBlock += " ";
      }

      currentBlock += line;
    }

    start = end + 1;

    if (end == markdown.length()) break;
  }
}

void MarkdownDoc::parseSpans(const String& text, MdBlock& block) {
  int i = 0;

  while (i < text.length()) {
    int boldStart = text.indexOf("**", i); //find first bold marker

    if (boldStart == -1) {  //if none, is regular text
      MdSpan span;
      span.type = SPAN_TEXT;
      span.text = text.substring(i);
      span.target = "";
      block.spans.push_back(span);
      break;
    }

    if (boldStart > i) { //if there is, previous text is normal
      MdSpan span;
      span.type = SPAN_TEXT;
      span.text = text.substring(i, boldStart);
      span.target = "";
      block.spans.push_back(span);
    }

    int boldEnd = text.indexOf("**", boldStart + 2);  //find matching end

    if (boldEnd == -1) { //if no end, ignore first bold maker and make it all normal
      MdSpan span;
      span.type = SPAN_TEXT;
      span.text = text.substring(boldStart);
      span.target = "";
      block.spans.push_back(span);
      break;
    }

    //contain stuff between bold markers as bold type
    MdSpan bold;
    bold.type = SPAN_BOLD;
    bold.text = text.substring(boldStart + 2, boldEnd);
    bold.target = "";
    block.spans.push_back(bold);

    //continue to next chunk after bold marker
    i = boldEnd + 2;
  }
}
