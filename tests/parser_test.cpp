#include "nodes.h"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace parser;

TEST_CASE("Test Node to string") {
  std::cout << "TESTING PARSER\n";
  ASTNode root(NodeType::PROGRAM);
  root.children.push_back(ASTNode(NodeType::STATEMENT_DROP_TABLE));
  root.children[0].children.push_back(ASTNode(NodeType::TABLE_REF, "foo"));
  std::string rep = root.ToString(0);
  INFO(rep);
  REQUIRE(rep ==
          "PROGRAM,\nChildren: [\n  STATEMENT_DROP_TABLE,\n  Children: [\n "
          "   TABLE_REF,\n    Value: 'foo'\n  ]\n]");
}
