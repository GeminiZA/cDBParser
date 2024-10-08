#pragma once

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace parser {

enum class NodeType {
  PROGRAM,
  STATEMENT_CREATE_TABLE,
  // STATEMENT_CREATE_VIEW,
  // STATEMENT_CREATE_INDEX,
  // STATEMENT_CREATE_SCHEMA,
  STATEMENT_ALTER_TABLE,
  // STATEMENT_ALTER_VIEW,
  // STATEMENT_ALTER_INDEX,
  STATEMENT_DROP_TABLE,
  // STATEMENT_DROP_VIEW,
  // STATEMENT_DROP_INDEX,
  // STATEMENT_DROP_SCHEMA,
  // STATEMENT_TRUNCATE,
  STATEMENT_SELECT,
  STATEMENT_INSERT,
  STATEMENT_UPDATE,
  STATEMENT_DELETE,
  // STATEMENT_BEGIN_TRANSACTION,
  // STATEMENT_COMMIT,
  // STATEMENT_ROLLBACK,
  // STATEMENT_SAVEPOINT,
  // STATEMENT_SET_TRANSACTION,
  // STATEMENT_EXPLAIN,
  // STATEMENT_SHOW,
  STATEMENT_COMMENT,
  // STATEMENT_MERGE,
  // STATEMENT_WITH,
  // STATEMENT_LOCK_TABLE,
  CLAUSE_SELECT,
  CLAUSE_FROM,
  CLAUSE_WHERE,
  EXPRESSION,
  BINARY_OPERATION,
  IDENTIFIER,
  TABLE_REF,
  COLUMN_REF,
};

std::string NodeTypeToString(NodeType type) {
  switch (type) {
  case NodeType::STATEMENT_CREATE_TABLE:
    return "STATEMENT_ALTER_TABLE";
    break;
  case NodeType::STATEMENT_ALTER_TABLE:
    return "STATEMENT_ALTER_TABLE";
    break;
  case NodeType::STATEMENT_DROP_TABLE:
    return "STATEMENT_DROP_TABLE";
    break;
  case NodeType::STATEMENT_SELECT:
    return "STATEMENT_SELECT";
    break;
  case NodeType::STATEMENT_INSERT:
    return "STATEMENT_INSERT";
    break;
  case NodeType::STATEMENT_UPDATE:
    return "STATEMENT_UPDATE";
    break;
  case NodeType::STATEMENT_DELETE:
    return "STATEMENT_DELETE";
    break;
  case NodeType::STATEMENT_COMMENT:
    return "STATEMENT_COMMENT";
    break;
  case NodeType::CLAUSE_SELECT:
    return "CLAUSE_SELECT";
    break;
  case NodeType::CLAUSE_FROM:
    return "CLAUSE_FROM";
    break;
  case NodeType::CLAUSE_WHERE:
    return "CLAUSE_WHERE";
    break;
  case NodeType::EXPRESSION:
    return "EXPRESSION";
    break;
  case NodeType::BINARY_OPERATION:
    return "BINARY_OPERATION";
    break;
  case NodeType::IDENTIFIER:
    return "IDENTIFIER";
    break;
  case NodeType::TABLE_REF:
    return "TABLE_REF";
    break;
  case NodeType::COLUMN_REF:
    return "COLUMN_REF";
    break;
  case NodeType::PROGRAM:
    return "PROGRAM";
    break;
  default:
    throw std::invalid_argument("Unknown node type");
  }
}

class ASTNode {
public:
  NodeType type;
  std::vector<ASTNode> children;
  ASTNode(NodeType type) : type(type) {};
  std::string ToString(int space) {
    std::stringstream ss;
    std::string spacer = "";
    for (int i = 0; i < space; i++) {
      spacer += " ";
    }
    ss << spacer << NodeTypeToString(type);
    if (children.size() > 0) {
      ss << spacer << ",\nChildren: [\n";
      for (int i = 0; i < children.size(); i++) {
        ss << children[i].ToString(space + 2) << ",\n";
      }
      ss << spacer << "]\n";
    } else {
    }
    return ss.str();
  }
};
} // namespace parser
