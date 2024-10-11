#include "tokens.h"
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace tokenizer {

Token::Token(TokenType type, std::string value, std::pair<size_t, size_t> pos)
    : type(type), value(value), position(pos) {}

Token::Token(TokenType type, std::pair<size_t, size_t> pos)
    : type(type), value(""), position(pos) {}

std::string Token::TypeString() const {
  switch (this->type) {
  case tokenizer::TokenType::KEYWORD_SELECT:
    return "KEYWORD_SELECT";
  case tokenizer::TokenType::KEYWORD_INSERT:
    return "KEYWORD_INSERT";
  case tokenizer::TokenType::KEYWORD_UPDATE:
    return "KEYWORD_UPDATE";
  case tokenizer::TokenType::KEYWORD_CREATE:
    return "KEYWORD_CREATE";
  case tokenizer::TokenType::KEYWORD_DROP:
    return "KEYWORD_DROP";
  case tokenizer::TokenType::KEYWORD_GROUP:
    return "KEYWORD_GROUP";
  case tokenizer::TokenType::KEYWORD_BY:
    return "KEYWORD_BY";
  case tokenizer::TokenType::KEYWORD_ORDER:
    return "KEYWORD_ORDER";
  case tokenizer::TokenType::KEYWORD_LIMIT:
    return "KEYWORD_LIMIT";
  case tokenizer::TokenType::KEYWORD_DELETE:
    return "KEYWORD_DELETE";
  case tokenizer::TokenType::KEYWORD_ALTER:
    return "KEYWORD_ALTER";
  case tokenizer::TokenType::KEYWORD_FROM:
    return "KEYWORD_FROM";
  case tokenizer::TokenType::KEYWORD_WHERE:
    return "KEYWORD_WHERE";
  case tokenizer::TokenType::KEYWORD_VALUES:
    return "KEYWORD_VALUES";
  case tokenizer::TokenType::KEYWORD_INTO:
    return "KEYWORD_INTO";
  case tokenizer::TokenType::KEYWORD_SET:
    return "KEYWORD_SET";
  case tokenizer::TokenType::KEYWORD_NULL:
    return "KEYWORD_NULL";
  case tokenizer::TokenType::KEYWORD_TABLE:
    return "KEYWORD_TABLE";
  case tokenizer::TokenType::KEYWORD_DATABASE:
    return "KEYWORD_DATABASE";
  case tokenizer::TokenType::KEYWORD_COLUMN:
    return "KEYWORD_COLUMN";
  case tokenizer::TokenType::KEYWORD_PRIMARY:
    return "KEYWORD_PRIMARY";
  case tokenizer::TokenType::KEYWORD_FOREIGN:
    return "KEYWORD_FOREIGN";
  case tokenizer::TokenType::KEYWORD_KEY:
    return "KEYWORD_KEY";
  case tokenizer::TokenType::KEYWORD_UNIQUE:
    return "KEYWORD_UNIQUE";
  case tokenizer::TokenType::OP_ADDITION:
    return "OP_ADDITION";
  case tokenizer::TokenType::OP_SUBTRACT:
    return "OP_SUBTRACT";
  case tokenizer::TokenType::OP_MULTIPLY:
    return "OP_MULTIPLY";
  case tokenizer::TokenType::OP_DIVIDE:
    return "OP_DIVIDE";
  case tokenizer::TokenType::OP_EQUAL:
    return "OP_EQUAL";
  case tokenizer::TokenType::OP_NOT_EQUAL:
    return "OP_NOT_EQUAL";
  case tokenizer::TokenType::OP_GREATER:
    return "OP_GREATER";
  case tokenizer::TokenType::OP_GREATER_EQUAL:
    return "OP_GREATER_EQUAL";
  case tokenizer::TokenType::OP_LESS:
    return "OP_LESS";
  case tokenizer::TokenType::OP_LESS_EQUAL:
    return "OP_LESS_EQUAL";
  case tokenizer::TokenType::OP_AND:
    return "OP_AND";
  case tokenizer::TokenType::OP_OR:
    return "OP_OR";
  case tokenizer::TokenType::OP_NOT:
    return "OP_NOT";
  case tokenizer::TokenType::TYPE_INT:
    return "TYPE_INT";
  case tokenizer::TokenType::TYPE_VARCHAR:
    return "TYPE_VARCHAR";
  case tokenizer::TokenType::TYPE_TEXT:
    return "TYPE_TEXT";
  case tokenizer::TokenType::TYPE_FLOAT:
    return "TYPE_FLOAT";
  case tokenizer::TokenType::TYPE_BOOLEAN:
    return "TYPE_BOOLEAN";
  case tokenizer::TokenType::TYPE_BLOB:
    return "TYPE_BLOB";
  case tokenizer::TokenType::LITERAL_INT:
    return "LITERAL_INT";
  case tokenizer::TokenType::LITERAL_FLOAT:
    return "LITERAL_FLOAT";
  case tokenizer::TokenType::LITERAL_STRING:
    return "LITERAL_STRING";
  case tokenizer::TokenType::LITERAL_BOOL:
    return "LITERAL_BOOL";
  case tokenizer::TokenType::IDENTIFIER:
    return "IDENTIFIER";
  case tokenizer::TokenType::PUNC_PAREN_OPEN:
    return "PUNC_PAREN_OPEN";
  case tokenizer::TokenType::PUNC_PAREN_CLOSE:
    return "PUNC_PAREN_CLOSE";
  case tokenizer::TokenType::PUNC_COMMA:
    return "PUNC_COMMA";
  case tokenizer::TokenType::PUNC_SEMICOLON:
    return "PUNC_SEMICOLON";
  case tokenizer::TokenType::COMMENT_SINGLE:
    return "COMMENT_SINGLE";
  case tokenizer::TokenType::COMMENT_MULTI:
    return "COMMENT_MULTI";
  default:
    throw std::invalid_argument("Uknown token type");
  }
}

std::string Token::ToString() const {
  std::stringstream ss;
  ss << "{" << this->TypeString() << ", ";
  if (this->value != "") {
    ss << "'" << this->value << "', ";
  }
  ss << "(" << this->position.first << ", " << this->position.second << ")}";
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << token.ToString();
  return os;
}

std::string TokensToStr(std::vector<Token> tokens) {
  std::stringstream ss;
  ss << "[\n";
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    ss << it->ToString() << "\n";
  }
  ss << "]\n";
  return ss.str();
}
} // namespace tokenizer
