#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace tokenizer {

enum class TokenType {
  KEYWORD_SELECT,
  KEYWORD_INSERT,
  KEYWORD_UPDATE,
  KEYWORD_CREATE,
  KEYWORD_DATABASE,
  KEYWORD_DROP,
  KEYWORD_GROUP,
  KEYWORD_BY,
  KEYWORD_AS,
  KEYWORD_TO,
  KEYWORD_ADD,
  KEYWORD_RENAME,
  KEYWORD_MODIFY,
  KEYWORD_ORDER,
  KEYWORD_LIMIT,
  KEYWORD_DELETE,
  KEYWORD_ALTER,
  KEYWORD_FROM,
  KEYWORD_WHERE,
  KEYWORD_VALUES,
  KEYWORD_INTO,
  KEYWORD_SET,
  KEYWORD_NULL,
  KEYWORD_TABLE,
  KEYWORD_DEFAULT,
  KEYWORD_COLUMN,
  KEYWORD_CONSTRAINT,
  KEYWORD_PRIMARY,
  KEYWORD_FOREIGN,
  KEYWORD_REFERENCES,
  KEYWORD_KEY,
  KEYWORD_UNIQUE,
  KEYWORD_ASC,
  KEYWORD_DESC,
  OP_ADDITION,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_EQUAL,
  OP_NOT_EQUAL,
  OP_GREATER,
  OP_GREATER_EQUAL,
  OP_LESS,
  OP_LESS_EQUAL,
  OP_AND,
  OP_OR,
  OP_NOT,
  TYPE_INT,
  TYPE_VARCHAR,
  TYPE_TEXT,
  TYPE_FLOAT,
  TYPE_BOOLEAN,
  TYPE_BLOB,
  LITERAL_INT,
  LITERAL_FLOAT,
  LITERAL_STRING,
  LITERAL_BOOL,
  IDENTIFIER,
  PUNC_PAREN_OPEN,
  PUNC_PAREN_CLOSE,
  PUNC_COMMA,
  PUNC_SEMICOLON,
  PUNC_PERIOD,
  COMMENT_SINGLE,
  COMMENT_MULTI,
};

class Token {
public:
  TokenType type;
  std::pair<size_t, size_t> position;
  std::string value;

  Token(TokenType type, std::string value, std::pair<size_t, size_t> pos);
  Token(TokenType type, std::pair<size_t, size_t> pos);
  std::string TypeString() const;
  std::string ToString() const;
  friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

std::string TokensToStr(std::vector<Token> tokens);
} // namespace tokenizer
