#include "parser.h"
#include "tokenizer.h"
#include "tokens.h"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

std::optional<std::string> loadFile(std::string path) {
  std::ifstream infile(path);

  if (!infile.is_open()) {
    std::cerr << "Failed to open file: " << path << "\n";
    return std::nullopt;
  }

  std::stringstream ss;
  ss << infile.rdbuf();
  return ss.str();
}

TEST_CASE("Test token type to string") {
  tokenizer::Token token(tokenizer::TokenType::LITERAL_STRING, "yes",
                         std::pair(0, 1));
  REQUIRE(token.TypeString() == "LITERAL_STRING");
}

TEST_CASE("Test token to string") {
  tokenizer::Token token(tokenizer::TokenType::LITERAL_STRING, "yes",
                         std::pair(0, 1));
  REQUIRE(token.ToString() == "{LITERAL_STRING, 'yes', (0, 1)}");
}

TEST_CASE("Test case 1_0.sql") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_0.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_SINGLE);
  REQUIRE(tokens[0].value == " Test case 0");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_CREATE);
  REQUIRE(tokens[2].type == tokenizer::TokenType::KEYWORD_TABLE);
  REQUIRE(tokens[3].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[3].value == "foo");
  REQUIRE(tokens[4].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[5].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[5].value == "bar");
  REQUIRE(tokens[6].type == tokenizer::TokenType::TYPE_INT);
  REQUIRE(tokens[7].type == tokenizer::TokenType::KEYWORD_PRIMARY);
  REQUIRE(tokens[8].type == tokenizer::TokenType::KEYWORD_KEY);
  REQUIRE(tokens[9].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[10].type == tokenizer::TokenType::PUNC_SEMICOLON);
}

TEST_CASE("Test case 1_1") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_1.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  std::stringstream ss;
  for (int i = 0; i < tokens.size(); i++) {
    ss << "Token[" << i << "] :" << tokens[i].ToString() << "\n";
  }
  INFO(ss.str());
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_MULTI);
  REQUIRE(tokens[0].value == "\nTest case 1\n");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_INSERT);
  REQUIRE(tokens[2].type == tokenizer::TokenType::KEYWORD_INTO);
  REQUIRE(tokens[3].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[3].value == "foo");
  REQUIRE(tokens[4].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[5].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[5].value == "bar");
  REQUIRE(tokens[6].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[7].type == tokenizer::TokenType::KEYWORD_VALUES);
  REQUIRE(tokens[8].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[9].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[9].value == "1");
  REQUIRE(tokens[10].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[11].type == tokenizer::TokenType::PUNC_SEMICOLON);

  REQUIRE(tokens[12].type == tokenizer::TokenType::KEYWORD_INSERT);
  REQUIRE(tokens[13].type == tokenizer::TokenType::KEYWORD_INTO);
  REQUIRE(tokens[14].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[14].value == "foo");
  REQUIRE(tokens[15].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[16].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[16].value == "bar");
  REQUIRE(tokens[17].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[18].type == tokenizer::TokenType::KEYWORD_VALUES);
  REQUIRE(tokens[19].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[20].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[20].value == "2");
  REQUIRE(tokens[21].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[22].type == tokenizer::TokenType::PUNC_SEMICOLON);

  REQUIRE(tokens[23].type == tokenizer::TokenType::KEYWORD_INSERT);
  REQUIRE(tokens[24].type == tokenizer::TokenType::KEYWORD_INTO);
  REQUIRE(tokens[25].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[25].value == "foo");
  REQUIRE(tokens[26].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[27].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[27].value == "bar");
  REQUIRE(tokens[28].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[29].type == tokenizer::TokenType::KEYWORD_VALUES);
  REQUIRE(tokens[30].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[31].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[31].value == "3");
  REQUIRE(tokens[32].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[33].type == tokenizer::TokenType::PUNC_SEMICOLON);
}

TEST_CASE("Test case 1_2.sql") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_2.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  std::stringstream ss;
  for (int i = 0; i < tokens.size(); i++) {
    ss << "Token[" << i << "] :" << tokens[i].ToString() << "\n";
  }
  INFO(ss.str());
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_SINGLE);
  REQUIRE(tokens[0].value == " Test case 2");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_SELECT);
  REQUIRE(tokens[2].type == tokenizer::TokenType::OP_MULTIPLY);
  REQUIRE(tokens[3].type == tokenizer::TokenType::KEYWORD_FROM);
  REQUIRE(tokens[4].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[4].value == "foo");
  REQUIRE(tokens[5].type == tokenizer::TokenType::PUNC_SEMICOLON);
}

TEST_CASE("Test case 1_3.sql") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_3.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  std::stringstream ss;
  for (int i = 0; i < tokens.size(); i++) {
    ss << "Token[" << i << "] :" << tokens[i].ToString() << "\n";
  }
  INFO(ss.str());
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_SINGLE);
  REQUIRE(tokens[0].value == " Test case 3");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_SELECT);
  REQUIRE(tokens[2].type == tokenizer::TokenType::OP_MULTIPLY);
  REQUIRE(tokens[3].type == tokenizer::TokenType::KEYWORD_FROM);
  REQUIRE(tokens[4].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[4].value == "foo");
  REQUIRE(tokens[5].type == tokenizer::TokenType::KEYWORD_WHERE);
  REQUIRE(tokens[6].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[6].value == "bar");
  REQUIRE(tokens[7].type == tokenizer::TokenType::OP_EQUAL);
  REQUIRE(tokens[8].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[8].value == "2");
  REQUIRE(tokens[9].type == tokenizer::TokenType::PUNC_SEMICOLON);
}

TEST_CASE("Test case 1_4.sql") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_4.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  std::stringstream ss;
  for (int i = 0; i < tokens.size(); i++) {
    ss << "Token[" << i << "] :" << tokens[i].ToString() << "\n";
  }
  INFO(ss.str());
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_SINGLE);
  REQUIRE(tokens[0].value == " Test case 4");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_UPDATE);
  REQUIRE(tokens[2].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[2].value == "foo");
  REQUIRE(tokens[3].type == tokenizer::TokenType::KEYWORD_SET);
  REQUIRE(tokens[4].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[4].value == "bar");
  REQUIRE(tokens[5].type == tokenizer::TokenType::OP_EQUAL);
  REQUIRE(tokens[6].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[6].value == "10");
  REQUIRE(tokens[7].type == tokenizer::TokenType::KEYWORD_WHERE);
  REQUIRE(tokens[8].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[8].value == "bar");
  REQUIRE(tokens[9].type == tokenizer::TokenType::OP_EQUAL);
  REQUIRE(tokens[10].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[10].value == "1");
  REQUIRE(tokens[11].type == tokenizer::TokenType::PUNC_SEMICOLON);
}

TEST_CASE("Test case 1_5") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_5.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  std::stringstream ss;
  for (int i = 0; i < tokens.size(); i++) {
    ss << "Token[" << i << "] :" << tokens[i].ToString() << "\n";
  }
  INFO(ss.str());
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_SINGLE);
  REQUIRE(tokens[0].value == " Test case 5");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_INSERT);
  REQUIRE(tokens[2].type == tokenizer::TokenType::KEYWORD_INTO);
  REQUIRE(tokens[3].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[3].value == "foo");
  REQUIRE(tokens[4].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[5].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[5].value == "bar");
  REQUIRE(tokens[6].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[7].type == tokenizer::TokenType::KEYWORD_VALUES);
  REQUIRE(tokens[8].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[9].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[9].value == "8");
  REQUIRE(tokens[10].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[11].type == tokenizer::TokenType::PUNC_SEMICOLON);

  REQUIRE(tokens[12].type == tokenizer::TokenType::KEYWORD_INSERT);
  REQUIRE(tokens[13].type == tokenizer::TokenType::KEYWORD_INTO);
  REQUIRE(tokens[14].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[14].value == "foo");
  REQUIRE(tokens[15].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[16].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[16].value == "bar");
  REQUIRE(tokens[17].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[18].type == tokenizer::TokenType::KEYWORD_VALUES);
  REQUIRE(tokens[19].type == tokenizer::TokenType::PUNC_PAREN_OPEN);
  REQUIRE(tokens[20].type == tokenizer::TokenType::LITERAL_INT);
  REQUIRE(tokens[20].value == "8");
  REQUIRE(tokens[21].type == tokenizer::TokenType::PUNC_PAREN_CLOSE);
  REQUIRE(tokens[22].type == tokenizer::TokenType::PUNC_SEMICOLON);
}

TEST_CASE("Test case 1_6") {
  std::optional<std::string> fileContents = loadFile("../tests/cases/1_6.sql");
  REQUIRE(fileContents.has_value());
  tokenizer::Tokenizer tokenizer(fileContents.value());
  tokenizer.Lex();
  std::vector<tokenizer::Token> tokens = tokenizer.GetTokens();
  std::stringstream ss;
  for (int i = 0; i < tokens.size(); i++) {
    ss << "Token[" << i << "] :" << tokens[i].ToString() << "\n";
  }
  INFO(ss.str());
  REQUIRE(tokens[0].type == tokenizer::TokenType::COMMENT_SINGLE);
  REQUIRE(tokens[0].value == " Test case 6");
  REQUIRE(tokens[1].type == tokenizer::TokenType::KEYWORD_DROP);
  REQUIRE(tokens[2].type == tokenizer::TokenType::KEYWORD_TABLE);
  REQUIRE(tokens[3].type == tokenizer::TokenType::IDENTIFIER);
  REQUIRE(tokens[3].value == "foo");
  REQUIRE(tokens[4].type == tokenizer::TokenType::PUNC_SEMICOLON);
}
