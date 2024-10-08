#pragma once

#include <ostream>
#include <vector>

namespace parser {

class ASTNode {
public:
  virtual void print(std::ostream &os) const = 0;
  friend std::ostream &operator<<(std::ostream &os, const ASTNode &node) {
    node.print(os);
    return os;
  }
};

enum class StatementType {
  CREATE_TABLE,
  // CREATE_VIEW,
  // CREATE_INDEX,
  // CREATE_SCHEMA,
  ALTER_TABLE,
  // ALTER_VIEW,
  // ALTER_INDEX,
  DROP_TABLE,
  // DROP_VIEW,
  // DROP_INDEX,
  // DROP_SCHEMA,
  // TRUNCATE,
  SELECT,
  INSERT,
  UPDATE,
  DELETE,
  // BEGIN_TRANSACTION,
  // COMMIT,
  // ROLLBACK,
  // SAVEPOINT,
  // SET_TRANSACTION,
  // EXPLAIN,
  // SHOW,
  COMMENT,
  // MERGE,
  // WITH,
  // LOCK_TABLE,
};

class StatementNode : ASTNode {
public:
  StatementType type;
};

class SelectStatementNode : StatementNode {
public:
  void print(std::ostream &os) const override { os << "SELECT_STATEMENT"; }
};

class ProgramNode : ASTNode {
public:
  std::vector<StatementNode> statements;
  void print(std::ostream &os) const override { os << "PROGRAM"; }
};

} // namespace parser
