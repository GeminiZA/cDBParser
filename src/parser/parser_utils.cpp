#include "parser_utils.h"
#include "tokens.h"

#include <exception>
#include <stdexcept>
#include <string>

using namespace parser;

std::exception UnexpectedToken(tokenizer::Token token) {
  return std::runtime_error("Unexpected token: " + token.ToString());
}

std::exception ParseError(tokenizer::Token token, std::string location) {
  return std::runtime_error("Error in (" + location +
                            ") parsing token: " + token.ToString());
}
