#include "nodes.h"
#include <iostream>

using namespace parser;

int main() {
  ASTNode root(NodeType::PROGRAM);
  std::cout << root.ToString(0);
}
