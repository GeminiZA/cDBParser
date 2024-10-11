#include "parser.h"
#include "nodes.h"
#include "parser_utils.h"
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

ASTNode Parser::ParseStatement(size_t &i) {
  ASTNode statement(NodeType::NULL_TYPE);
  switch (tokens[i].type) {
  case tokenizer::TokenType::KEYWORD_SELECT: {
    i++;
    statement = ASTNode(NodeType::STATEMENT_SELECT);
    ASTNode selectExpressionList = ParseSelectExpressionList(i);
    statement.children.push_back(selectExpressionList);
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_FROM:
      i++;
      break;
    default:
      throw UnexpectedToken(tokens[i]);
    }
    ASTNode tableList = ParseTableList(i);
    statement.children.push_back(tableList);
    // WHERE CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) {
      ASTNode clause = ParseWhereClause(i);
      statement.children.push_back(clause);
    }
    // GROUP BY CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_GROUP) {
      ASTNode clause = ParseGroupByClause(i);
      statement.children.push_back(clause);
    }
    // ORDER BY CLUASE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_ORDER) {
      ASTNode clause = ParseOrderByClause(i);
      statement.children.push_back(clause);
    }
    // LIMIT CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_GROUP) {
      ASTNode clause = ParseLimitClause(i);
      statement.children.push_back(clause);
    }
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_INSERT: {
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_INTO) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    statement = ASTNode(NodeType::STATEMENT_INSERT);
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
      throw UnexpectedToken(tokens[i]);
    }
    ASTNode tableRef = ASTNode(NodeType::TABLE_REF, tokens[i].value);
    statement.children.push_back(tableRef);
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    ASTNode columnList = ParseColumnList(i);
    statement.children.push_back(columnList);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_VALUES) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    ASTNode valueList = ParseValueList(i);
    statement.children.push_back(valueList);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_UPDATE: {
    i++;
    statement = ASTNode(NodeType::STATEMENT_UPDATE);
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
      throw UnexpectedToken(tokens[i]);
    }
    ASTNode tableRef = ASTNode(NodeType::TABLE_REF, tokens[i].value);
    statement.children.push_back(tableRef);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_SET) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    ASTNode AssignmentList = ParseAssignmentList(i);
    statement.children.push_back(AssignmentList);
    // WHERE CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) {
      ASTNode clause = ParseWhereClause(i);
      statement.children.push_back(clause);
    }
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_CREATE: {
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TABLE: {
      statement = ASTNode(NodeType::STATEMENT_CREATE_TABLE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
        throw UnexpectedToken(tokens[i]);
      }
      ASTNode tableRef(NodeType::TABLE_REF, tokens[i].value);
      statement.children.push_back(tableRef);
      i++;
      if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
        throw UnexpectedToken(tokens[i]);
      }
      i++;
      ASTNode columnDefList = ParseColumnDefList(i);
      statement.children.push_back(columnDefList);
      if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
        throw UnexpectedToken(tokens[i]);
      }
      i++;
      return statement;
    }
    case tokenizer::TokenType::KEYWORD_DATABASE: {
      throw UnexpectedToken(tokens[i]);
    }
    default:
      throw UnexpectedToken(tokens[i]);
    }
  }
  case tokenizer::TokenType::KEYWORD_DROP: { // Unfinished
    i++;
    if (i >= tokens.size()) {
      throw std::out_of_range("Unexpected EOF after token: " +
                              tokens[i].ToString());
    }
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TABLE:
      i++;
      statement = ASTNode(NodeType::STATEMENT_DROP_TABLE);
      break;
    case tokenizer::TokenType::KEYWORD_DATABASE:
      i++;
      statement = ASTNode(NodeType::STATEMENT_DROP_DATABASE);
      break;
    default:
      throw std::invalid_argument("Unexpected token: " + tokens[i].ToString());
    }
    break;
  }
  case tokenizer::TokenType::KEYWORD_DELETE: { // Unfinished
    statement = ASTNode(NodeType::STATEMENT_DELETE);
    break;
  }
  default:
    throw std::invalid_argument("Unexpected Token: " + tokens[i].ToString());
  }
  return statement;
}

ASTNode ParseSelectExpressionList(size_t &i) {
  return ASTNode(NodeType::NULL_TYPE);
}
} // namespace parser
