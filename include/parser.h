#pragma once

#include "nodes.h"
#include "tokens.h"

#include <optional>
#include <vector>
namespace parser {

class Parser {
private:
  std::vector<tokenizer::Token> tokens;
  std::optional<ASTNode> root;
  std::optional<ASTNode> ParseStatement(size_t &i);

public:
  Parser(std::vector<tokenizer::Token> tokens);
  void TryParse();
  std::optional<ASTNode> GetRoot();
};

} // namespace parser
