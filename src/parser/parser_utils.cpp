#include "parser_utils.h"
#include "tokens.h"

#include <stdexcept>
#include <string>

namespace parser {

std::runtime_error UnexpectedToken(tokenizer::Token token,
                                   std::string location) {
  return std::runtime_error("Unexpected token: " + token.ToString() + " in (" +
                            location + ")");
}

std::runtime_error ParseError(tokenizer::Token token, std::string location) {
  return std::runtime_error("Error in (" + location +
                            ") parsing token: " + token.ToString());
}

} // namespace parser
