#include "tokenizer.h"
#include "tokens.h"
#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <strings.h>

namespace tokenizer {

Tokenizer::Tokenizer(std::string input) : raw(input) {
  parseLines();
  /* std::cout << "Line ends: \n";
  for (int i = 0; i < lineEnds.size(); i++) {
    std::cout << "Pos: " << getPosStr(lineEnds[i]) << " i: " << lineEnds[i]
              << " Char: " << int(raw[lineEnds[i]]) << '\n';
  }
  std::cout << "End\n";
  std::cout << "Pos of 16: " << getPosStr(16) << "\n"; */
}

void Tokenizer::parseLines() {
  for (int i = 0; i < raw.size(); i++) {
    if (raw[i] == '\n') {
      lineEnds.push_back(i + 1);
    }
  }
}

std::pair<size_t, size_t> Tokenizer::getPos(size_t index) const {
  size_t line = 0;
  while (line < lineEnds.size()) {
    if (lineEnds[line] > index) {
      break;
    }
    line++;
  }
  if (line > 0) {
    return std::pair(line, index - lineEnds[line - 1]);
  } else {
    return std::pair(0, index);
  }
}

std::string Tokenizer::getPosStr(size_t index) const {
  std::pair<size_t, size_t> pos = getPos(index);
  return "(" + std::to_string(pos.first) + ":" + std::to_string(pos.second) +
         ")";
}

const std::string punc = " ,();\n+-*/><=!.";

bool Tokenizer::isPunc(char cur, std::string punc) {
  size_t i = punc.find(cur);
  return i != std::string::npos;
}

bool Tokenizer::compWord(std::string a, std::string b) {
  if (a.size() != b.size())
    return false;
  for (int i = 0; i < a.size(); i++) {
    if (std::toupper(a[i]) != std::toupper(b[i]))
      return false;
  }
  return true;
}

void Tokenizer::Lex() {
  size_t i = 0;
  while (i < raw.size()) {
    // std::cout << "Starting to lex token starting at: " << getPosStr(i)
    //<< "Char: " << int(raw[i]) << "\n";
    // punctuation
    while (i < raw.size() && (raw[i] == ' ' || raw[i] == '\t' ||
                              raw[i] == '\n' || raw[i] == '\r'))
      i++;
    if (i == raw.size()) {
      break;
    }
    size_t start = i;
    // std::cout << "Skipping whitespace... Now at: " << getPosStr(i)
    // << " Char: " << int(raw[i]) << "\n";
    if (raw[i] == ',') {
      Token newToken = Token(TokenType::PUNC_COMMA, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == '.') {
      Token newToken = Token(TokenType::PUNC_PERIOD, getPos(i));
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == ';') {
      Token newToken = Token(TokenType::PUNC_SEMICOLON, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == '(') {
      Token newToken = Token(TokenType::PUNC_PAREN_OPEN, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == ')') {
      Token newToken = Token(TokenType::PUNC_PAREN_CLOSE, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == '+') {
      Token newToken = Token(TokenType::OP_ADDITION, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == '-') {
      if (i + 1 < raw.size() && raw[i + 1] == '-') {
        size_t start = i;
        i += 2;
        std::string val = "";
        while (i < raw.size() && raw[i] != '\n') {
          val += raw[i];
          i++;
        } // i will be on \n
        Token newToken = Token(TokenType::COMMENT_SINGLE, val, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i++;
        continue;
      } else {
        Token newToken = Token(TokenType::OP_SUBTRACT, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i++;
        continue;
      }
    } else if (raw[i] == '*') {
      Token newToken = Token(TokenType::OP_MULTIPLY, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == '/') {
      if (i + 1 < raw.size() && raw[i + 1] == '*') {
        size_t start = i;
        i += 2;
        std::string val = "";
        while (i + 1 < raw.size() && !(raw[i] == '*' && raw[i + 1] == '/')) {
          val += raw[i];
          i++;
        }
        if (i == raw.size() - 1) {
          throw std::invalid_argument("Unterminated multi line comment at: " +
                                      getPosStr(start));
        }
        Token newToken = Token(TokenType::COMMENT_MULTI, val, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i += 2;

        continue;
      } else {
        Token newToken = Token(TokenType::OP_DIVIDE, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i++;
        continue;
      }
    } else if (raw[i] == '>') {
      if (i + 1 < raw.size() && raw[i + 1] == '=') {
        Token newToken = Token(TokenType::OP_GREATER_EQUAL, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i += 2;
        continue;
      } else {
        Token newToken = Token(TokenType::OP_GREATER, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i++;
        continue;
      }
    } else if (raw[i] == '<') {
      if (i + 1 < raw.size() && raw[i + 1] == '=') {
        Token newToken = Token(TokenType::OP_LESS_EQUAL, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i += 2;
        continue;
      } else if (i + 1 < raw.size() && raw[i + 1] == '>') {
        Token newToken = Token(TokenType::OP_NOT_EQUAL, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i += 2;
        continue;
      } else {
        Token newToken = Token(TokenType::OP_LESS, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i++;
        continue;
      }
    } else if (raw[i] == '=') {
      Token newToken = Token(TokenType::OP_EQUAL, getPos(i));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++;
      continue;
    } else if (raw[i] == '!') {
      if (i + 1 < raw.size() && raw[i + 1] == '=') {
        Token newToken = Token(TokenType::OP_NOT_EQUAL, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i += 2;
        continue;
      } else {
        Token newToken = Token(TokenType::OP_NOT, getPos(i));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        i++;
        continue;
      }
    } else if (isdigit(raw[i])) { // Number literal
      size_t start = i;
      std::string val = "";
      while (i < raw.size() && !isPunc(raw[i], punc)) {
        if (!isdigit(raw[i]) && raw[i] != '.') {
          throw std::invalid_argument("Invalid number literal at: " +
                                      getPosStr(start));
        }
        val += raw[i];
        i++; // i will be after the number
      }
      // Only one period
      auto numPeriods = std::ranges::count(val, '.');
      if (numPeriods > 1) {
        throw std::invalid_argument("Invalid number literal at: " +
                                    getPosStr(start));
      } else if (numPeriods == 0) {
        Token newToken = Token(TokenType::LITERAL_INT, val, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else {
        Token newToken = Token(TokenType::LITERAL_FLOAT, val, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      }
    } else if (raw[i] == '\"') {
      std::string val = "";
      i++;
      while (i < raw.size() && raw[i] != '\"') {
        val += raw[i];
        i++;
      }
      if (i == raw.size())
        throw std::out_of_range("Unterminated string literal at: " +
                                getPosStr(i));
      Token newToken(TokenType::LITERAL_STRING, val, getPos(start));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++; // push i after closing quote
      continue;
    } else if (raw[i] == '\'') {
      std::string val = "";
      i++;
      while (i < raw.size() && raw[i] != '\'') {
        val += raw[i];
        i++;
      }
      if (i == raw.size())
        throw std::out_of_range("Unterminated string literal at: " +
                                getPosStr(i));
      Token newToken(TokenType::LITERAL_STRING, val, getPos(start));
      // std::cout << "Lexed token: " << newToken.ToString() << "\n";
      tokens.push_back(newToken);
      i++; // push i after closing quote
      continue;
    } else {
      size_t start = i;
      std::string val = "";
      while (i < raw.size() && !isPunc(raw[i], punc)) {
        val += raw[i];
        i++;
      } // i will be after the val at end
      // Check for keywords
      if (compWord(val, "SELECT")) {
        Token newToken = Token(TokenType::KEYWORD_SELECT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
        ;
      } else if (compWord(val, "AS")) {
        Token newToken = Token(TokenType::KEYWORD_AS, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "INSERT")) {
        Token newToken = Token(TokenType::KEYWORD_INSERT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "UPDATE")) {
        Token newToken = Token(TokenType::KEYWORD_UPDATE, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "CREATE")) {
        Token newToken = Token(TokenType::KEYWORD_CREATE, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "DROP")) {
        Token newToken = Token(TokenType::KEYWORD_DROP, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "GROUP")) {
        Token newToken = Token(TokenType::KEYWORD_GROUP, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "BY")) {
        Token newToken = Token(TokenType::KEYWORD_BY, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "ORDER")) {
        Token newToken = Token(TokenType::KEYWORD_ORDER, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "LIMIT")) {
        Token newToken = Token(TokenType::KEYWORD_LIMIT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "DELETE")) {
        Token newToken = Token(TokenType::KEYWORD_DELETE, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "ALTER")) {
        Token newToken = Token(TokenType::KEYWORD_ALTER, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "RENAME")) {
        Token newToken = Token(TokenType::KEYWORD_RENAME, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "ADD")) {
        Token newToken = Token(TokenType::KEYWORD_ADD, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "TO")) {
        Token newToken = Token(TokenType::KEYWORD_TO, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "MODIFY")) {
        Token newToken = Token(TokenType::KEYWORD_MODIFY, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "FROM")) {
        Token newToken = Token(TokenType::KEYWORD_FROM, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "WHERE")) {
        Token newToken = Token(TokenType::KEYWORD_WHERE, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "VALUES")) {
        Token newToken = Token(TokenType::KEYWORD_VALUES, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "INTO")) {
        Token newToken = Token(TokenType::KEYWORD_INTO, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "SET")) {
        Token newToken = Token(TokenType::KEYWORD_SET, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "NULL")) {
        Token newToken = Token(TokenType::KEYWORD_NULL, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "TABLE")) {
        Token newToken = Token(TokenType::KEYWORD_TABLE, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "COLUMN")) {
        Token newToken = Token(TokenType::KEYWORD_COLUMN, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "UNIQUE")) {
        Token newToken = Token(TokenType::KEYWORD_UNIQUE, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "PRIMARY")) {
        Token newToken = Token(TokenType::KEYWORD_PRIMARY, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "FOREIGN")) {
        Token newToken = Token(TokenType::KEYWORD_FOREIGN, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "KEY")) {
        Token newToken = Token(TokenType::KEYWORD_KEY, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "INT")) {
        Token newToken = Token(TokenType::TYPE_INT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "INTEGER")) {
        Token newToken = Token(TokenType::TYPE_INT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "VARCHAR")) {
        Token newToken = Token(TokenType::TYPE_VARCHAR, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "TEXT")) {
        Token newToken = Token(TokenType::TYPE_TEXT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "FLOAT")) {
        Token newToken = Token(TokenType::TYPE_FLOAT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "DECIMAL")) {
        Token newToken = Token(TokenType::TYPE_FLOAT, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "BOOLEAN")) {
        Token newToken = Token(TokenType::TYPE_BOOLEAN, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "BOOL")) {
        Token newToken = Token(TokenType::TYPE_BOOLEAN, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "BLOB")) {
        Token newToken = Token(TokenType::TYPE_BLOB, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "TRUE")) {
        Token newToken = Token(TokenType::LITERAL_BOOL, "TRUE", getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else if (compWord(val, "FALSE")) {
        Token newToken = Token(TokenType::LITERAL_BOOL, "FALSE", getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      } else { // Should be an identifier
        Token newToken = Token(TokenType::IDENTIFIER, val, getPos(start));
        // std::cout << "Lexed token: " << newToken.ToString() << "\n";
        tokens.push_back(newToken);
        continue;
      }
    }
  }
}

std::vector<Token> Tokenizer::GetTokens() const { return this->tokens; }

} // namespace tokenizer
