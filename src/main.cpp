#include "tokenizer.h"
#include "tokens.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  std::string dir = "../tests/cases/";
  std::vector<std::string> paths;
  for (const auto &entry : std::filesystem::directory_iterator(dir)) {
    paths.push_back(entry.path());
  }
  std::sort(paths.begin(), paths.end());
  for (const auto &it : paths) {
    std::cout << "File: " << it << "\n";
    std::ifstream infile(it);

    if (!infile.is_open()) {
      std::cerr << "Failed to open file: " << it << "\n";
      return 1;
    }

    std::stringstream ss;
    ss << infile.rdbuf();
    std::string fileContents = ss.str();
    std::cout << "Contents:\n==============================\n"
              << fileContents << "==============================\n";

    infile.close();

    tokenizer::Tokenizer tknzr(fileContents);
    tknzr.Lex();
    std::cout << "Tokenized:\n";
    std::cout << tokenizer::TokensToStr(tknzr.GetTokens());
  }
}
