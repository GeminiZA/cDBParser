#pragma once

#include "tokens.h"
#include <cstddef>
#include <string>
#include <vector>

namespace tokenizer {

class Tokenizer {

private:
  std::vector<size_t> lineEnds;
  std::string raw;
  std::vector<Token> tokens;
  void parseLines();
  std::pair<size_t, size_t> getPos(size_t index) const;
  std::string getPosStr(size_t index) const;
  bool isPunc(char cur, std::string punc);
  bool compWord(std::string a, std::string b);

public:
  void Lex();
  Tokenizer(std::string input);
  std::vector<Token> GetTokens() const;
};

} // namespace tokenizer
