#include "parser.h"
#include "nodes.h"
#include "tokens.h"
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

namespace parser {

Parser::Parser(std::vector<tokenizer::Token> tokens) : tokens(tokens) {};

std::optional<ASTNode> Parser::GetRoot() { return root; }

void Parser::TryParse() {
  size_t i = 0;
  while (i < tokens.size()) {
    try {
      ParseStatement(i);
    } catch (std::exception &e) {
      std::cerr << "Error Parsing statement: " << e.what();
      return;
    }
  }
}

std::optional<ASTNode> Parser::ParseStatement(size_t &i) {
  ASTNode statement(NodeType::NULL_TYPE);
  switch (tokens[i].type) {
  case tokenizer::TokenType::COMMENT_MULTI:
    break;
  case tokenizer::TokenType::COMMENT_SINGLE:
    break;
  case tokenizer::TokenType::KEYWORD_DROP:
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::COMMENT_MULTI:
      i++;
    }

    break;
  default:
    throw std::invalid_argument("Unexpected Token: " + tokens[i].ToString());
  }
  if (statement.type == NodeType::NULL_TYPE) {
    return std::nullopt;
  } else {
    return std::optional(statement);
  }
}
} // namespace parser
