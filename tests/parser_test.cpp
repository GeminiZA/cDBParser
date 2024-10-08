#include "parser.h"
#include "tokenizer.h"
#include "tokens.h"
#include <catch2/catch_test_macros.hpp>

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
