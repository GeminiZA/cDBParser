#include "nodes.h"
#include "parser.h"
#include "tokenizer.h"
#include <iostream>
#include <optional>

int main() {
  const std::string eg =
      "/*\nTest case1\n*/\n\nINSERT INTO foo (bar)\nVALUES (1);\n";
  std::cout << "Trying to tokenizer:\n" << eg << "\n";
  tokenizer::Tokenizer tokenizer(eg);
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  for (int i = 0; i < tokens.size(); i++) {
    std::cout << "(" << i << ") \t" << tokens[i] << "\n";
  }
  std::cout << "Trying to parser:\n";
  parser::Parser parser(tokens);
  parser.TryParse();
  std::optional<parser::ASTNode> root = parser.GetRoot();
  if (root.has_value()) {
    std::cout << root.value().ToString(0) << "\n";
  } else {
    std::cout << "No root node\n";
  }
}
