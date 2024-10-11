#pragma once

#include "tokens.h"
#include <exception>
#include <iostream>

namespace parser {
std::exception UnexpectedToken(tokenizer::Token);
std::exception ParseError(tokenizer::Token, std::string location);
} // namespace parser
