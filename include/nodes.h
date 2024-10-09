#pragma once

#include <optional>
#include <string>
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
  UNARY_OPERATION,
  IDENTIFIER,
  TABLE_REF,
  COLUMN_REF,
  NULL_TYPE,
};

std::string NodeTypeToString(NodeType type);

enum class BinaryOperator {
  SUBTRACT,
  DIVIDE,
  MULTIPLY,
  ADD,
  OR,
  AND,
  LIKE,
};

enum class UnaryOperator { NOT };

class ASTNode {
public:
  NodeType type;
  std::vector<ASTNode> children;
  ASTNode(NodeType type) : type(type), value(std::nullopt) {};
  ASTNode(NodeType type, std::string value)
      : type(type), value(std::optional(value)) {};
  std::string ToString(int space);
  std::optional<std::string> value;
};
} // namespace parser
