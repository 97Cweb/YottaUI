#pragma once 
#include <vector>
#include <Arduino.h>

enum SpanType {
  SPAN_TEXT,
  SPAN_LINK,
  SPAN_BOLD,
  SPAN_ITALIC,
  SPAN_CODE
};

struct MdSpan {
  SpanType type;
  String text;
  String target; // only used for links/images if needed
};

enum MdBlockType {
  MD_PARAGRAPH,
  MD_HEADING,
  MD_IMAGE,
  MD_TABLE,
  MD_CODE,
  MD_RULE
};

struct MdBlock {
  MdBlockType type;
  std::vector<MdSpan> spans;
  String target;
};

class MarkdownDoc{
public:
  MarkdownDoc(String markdown){
    parse(markdown);
  }

  const std::vector<MdBlock>& getBlocks() const {
    return _blocks;
}

private:
  std::vector<MdBlock> _blocks;

  void parse(const String& markdown);
};
