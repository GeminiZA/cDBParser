#include "nodes.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace parser {

std::string NodeTypeToString(NodeType type) {
  switch (type) {
  case NodeType::STATEMENT_CREATE_TABLE:
    return "STATEMENT_ALTER_TABLE";
  case NodeType::STATEMENT_ALTER_TABLE:
    return "STATEMENT_ALTER_TABLE";
  case NodeType::STATEMENT_DROP_TABLE:
    return "STATEMENT_DROP_TABLE";
  case NodeType::STATEMENT_SELECT:
    return "STATEMENT_SELECT";
  case NodeType::STATEMENT_INSERT:
    return "STATEMENT_INSERT";
  case NodeType::STATEMENT_UPDATE:
    return "STATEMENT_UPDATE";
  case NodeType::STATEMENT_DELETE:
    return "STATEMENT_DELETE";
  case NodeType::STATEMENT_COMMENT:
    return "STATEMENT_COMMENT";
  case NodeType::CLAUSE_SELECT:
    return "CLAUSE_SELECT";
  case NodeType::CLAUSE_FROM:
    return "CLAUSE_FROM";
  case NodeType::CLAUSE_WHERE:
    return "CLAUSE_WHERE";
  case NodeType::EXPRESSION:
    return "EXPRESSION";
  case NodeType::BINARY_OPERATION:
    return "BINARY_OPERATION";
  case NodeType::UNARY_OPERATION:
    return "UNARY_OPERATION";
  case NodeType::IDENTIFIER:
    return "IDENTIFIER";
  case NodeType::TABLE_REF:
    return "TABLE_REF";
  case NodeType::COLUMN_REF:
    return "COLUMN_REF";
  case NodeType::PROGRAM:
    return "PROGRAM";
  default:
    throw std::invalid_argument("Unknown node type");
  }
}

std::string ASTNode::ToString(int space) {
  std::stringstream ss;
  std::string spacer = "";
  for (int i = 0; i < space; i++) {
    spacer += " ";
  }
  ss << spacer << NodeTypeToString(type);
  if (value.has_value()) {
    ss << ",\n" << spacer << "Value: '" << value.value() << "'";
  }
  if (children.size() > 0) {
    ss << ",\n" << spacer << "Children: [\n";
    for (int i = 0; i < children.size() - 1; i++) {
      ss << children[i].ToString(space + 2) << ",\n";
    }
    ss << children[children.size() - 1].ToString(space + 2) << "\n";
    ss << spacer << "]";
  }
  return ss.str();
}

} // namespace parser
