#pragma once 
#include "MarkdownDoc.h"


void MarkdownDoc::parse(const String& markdown){
  _blocks.clear();
  
  MdSpan span;
  span.type = SPAN_TEXT;
  span.text = markdown;

  MdBlock block;
  block.type = MD_PARAGRAPH;
  block.spans.push_back(span);

  _blocks.push_back(block);
}
