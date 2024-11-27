#pragma once

#include "tokens.h"
#include <stdexcept>

namespace parser {
std::runtime_error UnexpectedToken(tokenizer::Token, std::string location);
std::runtime_error ParseError(tokenizer::Token, std::string location);
} // namespace parser
