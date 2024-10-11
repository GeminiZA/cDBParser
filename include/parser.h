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
  ASTNode ParseStatement(size_t &i);
  ASTNode ParseSelectExpressionList(size_t &i);
  ASTNode ParseTableList(size_t &i);
  ASTNode ParseWhereClause(size_t &i);
  ASTNode ParseGroupByClause(size_t &i);
  ASTNode ParseOrderByClause(size_t &i);
  ASTNode ParseLimitClause(size_t &i);
  ASTNode ParseColumnList(size_t &i);
  ASTNode ParseColumnDefList(size_t &i);
  ASTNode ParseValueList(size_t &i);
  ASTNode ParseAssignmentList(size_t &i);

public:
  Parser(std::vector<tokenizer::Token> tokens);
  void TryParse();
  std::optional<ASTNode> GetRoot();
};

} // namespace parser
