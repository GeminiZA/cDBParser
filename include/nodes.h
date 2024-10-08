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

enum class ExpressionType {
  TABLE_REF,
};

class Expression : public ASTNode {
public:
  ExpressionType type;
};

class TableRefExpression : public Expression {
public:
}

class ColumnRef {
public:
  std::string name;
  ColumnRef(std::string name) : name(name) {};
};

enum class ClauseType {
  SELECT,
  FROM,
  WHERE,
};

class ClauseNode : public ASTNode {
public:
  ClauseType type;
  ClauseNode(ClauseType type) : type(type) {};
};

class SelectClauseNode : public ClauseNode {
public:
  SelectClauseNode() : ClauseNode(ClauseType::SELECT) {};
  std::vector<ColumnRef> columns;
  void AddColumn(std::string name) { columns.push_back(ColumnRef(name)); }
  std::vector<ColumnRef> GetColumns() { return columns; }
};

class FromClauseNode : public ClauseNode {
public:
  FromClauseNode(Expression expr) : ClauseNode(ClauseType::FROM), expr(expr) {};
  Expression expr;
}

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
  std::vector<ClauseNode> clauses;
  StatementType type;
};

class SelectStatementNode : StatementNode {
public:
  SelectStatementNode() { type = StatementType::SELECT; }
  void print(std::ostream &os) const override { os << "SELECT_STATEMENT"; }
};

class ProgramNode : ASTNode {
public:
  std::vector<StatementNode> statements;
  void print(std::ostream &os) const override { os << "PROGRAM"; }
};

} // namespace parser
