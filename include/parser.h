#pragma once

#include "nodes.h"
#include "tokens.h"

#include <vector>
namespace parser {

class Parser {
private:
  std::vector<tokenizer::Token> tokens;

public:
  Parser(std::vector<tokenizer::Token> tokens);
  void Parse();
  ProgramNode GetRoot();
};

} // namespace parser
