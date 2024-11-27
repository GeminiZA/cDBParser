#include "parser.h"
#include "nodes.h"
#include "parser_utils.h"
#include "tokens.h"
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

namespace parser {

Parser::Parser(std::vector<tokenizer::Token> tokens) {
  std::vector<tokenizer::Token> filtered;
  for (int i = 0; i < tokens.size(); i++) {
    if (tokens[i].type == tokenizer::TokenType::COMMENT_SINGLE ||
        tokens[i].type == tokenizer::TokenType::COMMENT_MULTI) {
      continue;
    } else {
      filtered.push_back(tokens[i]);
    }
  }
  this->tokens = filtered;
};

std::optional<ASTNode> Parser::GetRoot() { return root; }

void Parser::TryParse() {
  size_t i = 0;
  while (i < tokens.size()) {
    try {
      root = ParseStatement(i);
    } catch (std::exception &e) {
      std::cerr << "Error Parsing statement: " << e.what() << "\n";
      return;
    }
  }
}

ASTNode Parser::ParseStatement(size_t &i) {
  std::cout << "Parsing Statement from: " << tokens[i] << "\n";
  ASTNode statement(NodeType::NULL_TYPE);
  switch (tokens[i].type) {
  case tokenizer::TokenType::KEYWORD_SELECT: { // select_statement
    i++;
    statement = ASTNode(NodeType::STATEMENT_SELECT);
    ASTNode selectExpressionList = ParseSelectExpressionList(i);
    statement.children.push_back(selectExpressionList);
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_FROM:
      i++;
      break;
    default:
      throw UnexpectedToken(tokens[i], "ParseStatment");
    }
    ASTNode tableList = ParseTableList(i);
    statement.children.push_back(tableList);
    // WHERE CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) {
      ASTNode clause = ParseWhereClause(i);
      statement.children.push_back(clause);
    }
    // GROUP BY CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_GROUP) {
      ASTNode clause = ParseGroupByClause(i);
      statement.children.push_back(clause);
    }
    // ORDER BY CLUASE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_ORDER) {
      ASTNode clause = ParseOrderByClause(i);
      statement.children.push_back(clause);
    }
    // LIMIT CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_GROUP) {
      ASTNode clause = ParseLimitClause(i);
      statement.children.push_back(clause);
    }
    if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_INSERT: { // insert_statement
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_INTO) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    statement = ASTNode(NodeType::STATEMENT_INSERT);
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    ASTNode columnList = ParseColumnList(i);
    statement.children.push_back(columnList);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_VALUES) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    ASTNode valueList = ParseValueList(i);
    statement.children.push_back(valueList);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_UPDATE: { // update_statement
    i++;
    statement = ASTNode(NodeType::STATEMENT_UPDATE);
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_SET) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    ASTNode AssignmentList = ParseAssignmentList(i);
    statement.children.push_back(AssignmentList);
    // WHERE CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) {
      ASTNode clause = ParseWhereClause(i);
      statement.children.push_back(clause);
    }
    if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_CREATE: { // create_statement
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TABLE: {
      statement = ASTNode(NodeType::STATEMENT_CREATE_TABLE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
        throw UnexpectedToken(tokens[i], "ParseStatement");
      }
      ASTNode tableRef = ParseTableRef(i);
      statement.children.push_back(tableRef);
      if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
        throw UnexpectedToken(tokens[i], "ParseStatement");
      }
      i++;
      ASTNode tableElementsList = ParseTableElementsList(i);
      statement.children.push_back(tableElementsList);
      if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
        throw UnexpectedToken(tokens[i], "ParseStatement");
      }
      i++;
      if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
        throw UnexpectedToken(tokens[i], "ParseStatement");
      }
      i++;
      return statement;
    }
    case tokenizer::TokenType::KEYWORD_DATABASE: {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    default:
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
  }
  case tokenizer::TokenType::KEYWORD_DELETE: { // delete_statement
    statement = ASTNode(NodeType::STATEMENT_DELETE);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_FROM)
      throw UnexpectedToken(tokens[i], "ParseStatement");
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i], "ParseStatement");
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) { // WHERE clause
      ASTNode clause = ParseWhereClause(i);
    }
    if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_ALTER: { // alter_statement
    statement = ASTNode(NodeType::STATEMENT_ALTER_TABLE);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_TABLE)
      throw UnexpectedToken(tokens[i], "ParseStatement");
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i], "ParseStatement");
    i++;
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    ASTNode alterActions = ParseAlterActionList(i);
    statement.children.push_back(alterActions);
    if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_DROP: { // drop_statement
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TABLE: {
      statement = ASTNode(NodeType::STATEMENT_DROP_TABLE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i], "ParseStatement");
      ASTNode tableRef = ParseTableRef(i);
      statement.children.push_back(tableRef);
      if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
        throw UnexpectedToken(tokens[i], "ParseStatement");
      }
      i++;
      return statement;
    }
    case tokenizer::TokenType::KEYWORD_COLUMN: {
      statement = ASTNode(NodeType::STATEMENT_DROP_COLUMN);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i], "ParseStatement");
      ASTNode columnRef = ParseColumnRef(i);
      statement.children.push_back(columnRef);
    }
    case tokenizer::TokenType::KEYWORD_DATABASE: {
      statement = ASTNode(NodeType::STATEMENT_DROP_DATABASE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i], "ParseStatement");
      ASTNode databaseRef(NodeType::DATABASE_REF, tokens[i].value);
      statement.children.push_back(databaseRef);
    }
    default:
      throw UnexpectedToken(tokens[i], "ParseStatement");
    }
  }
  default:
    throw std::invalid_argument("Unexpected Token: " + tokens[i].ToString());
  }
  if (tokens[i].type != tokenizer::TokenType::PUNC_SEMICOLON) {
    throw UnexpectedToken(tokens[i], "ParseStatement");
  }
  i++;
  return statement;
}

ASTNode Parser::ParseSelectExpressionList(size_t &i) {
  std::cout << "Parsing SelectExpressionList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::SELECT_LIST);
  if (tokens[i].type == tokenizer::TokenType::OP_MULTIPLY) {
    list.children.push_back(ASTNode(NodeType::COLUMNS_ALL));
    i++;
    return list;
  } else {
    ASTNode columnList = ParseColumnList(i);
    list.children.push_back(columnList);
    return list;
  }
}

ASTNode Parser::ParseTableList(size_t &i) {
  std::cout << "Parsing TableList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::TABLE_LIST);
  ASTNode tableRef = ParseTableRef(i);
  list.children.push_back(tableRef);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode tableRef = ParseTableRef(i);
    list.children.push_back(tableRef);
  }
  return list;
}

ASTNode Parser::ParseWhereClause(size_t &i) {
  std::cout << "Parsing WhereClause from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_WHERE)
    throw UnexpectedToken(tokens[i], "ParseWhereClause");
  ASTNode clause(NodeType::CLAUSE_WHERE);
  i++;
  ASTNode condition = ParseLogicalExpression(i);
  clause.children.push_back(condition);
  return clause;
}

ASTNode Parser::ParseGroupByClause(size_t &i) {
  std::cout << "Parsing GroupByClause from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_GROUP)
    throw UnexpectedToken(tokens[i], "ParseGroupByClause");
  i++;
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_BY)
    throw UnexpectedToken(tokens[i], "ParseGroupByClause");
  i++;
  ASTNode clause(NodeType::CLAUSE_GROUP_BY);
  ASTNode cols = ParseColumnList(i);
  clause.children.push_back(cols);
  return clause;
}

ASTNode Parser::ParseOrderByClause(size_t &i) {
  std::cout << "Parsing OrderByClause from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_ORDER)
    throw UnexpectedToken(tokens[i], "ParseOrderByClause");
  i++;
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_BY)
    throw UnexpectedToken(tokens[i], "ParseOrderByClause");
  i++;
  ASTNode clause(NodeType::CLAUSE_ORDER_BY);
  ASTNode orderList = ParseOrderList(i);
  clause.children.push_back(orderList);
  return clause;
}

ASTNode Parser::ParseLimitClause(size_t &i) {
  std::cout << "Parsing LimitClause from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_LIMIT)
    throw UnexpectedToken(tokens[i], "ParseLimitClause");
  i++;
  ASTNode clause(NodeType::CLAUSE_LIMIT);
  if (tokens[i].type != tokenizer::TokenType::LITERAL_INT)
    throw UnexpectedToken(tokens[i], "ParseLimitClause");
  clause.children.push_back(ASTNode(NodeType::LITERAL_INT, tokens[i].value));
  i++;
  return clause;
}

ASTNode Parser::ParseColumnList(size_t &i) {
  std::cout << "Parsing ColumnList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::COLUMN_LIST);
  ASTNode colRef = ParseColumnRef(i);
  list.children.push_back(colRef);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode colRef = ParseColumnRef(i);
    list.children.push_back(colRef);
  }
  return list;
}

ASTNode Parser::ParseColumnDefList(size_t &i) {
  std::cout << "Parsing ColumnDefList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::COLUMN_DEFINITION_LIST);
  ASTNode colDef = ParseColumnDef(i);
  list.children.push_back(colDef);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode colDef = ParseColumnDef(i);
    list.children.push_back(colDef);
  }
  return list;
}

ASTNode Parser::ParseValueList(size_t &i) {
  std::cout << "Parsing ValueList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::VALUE_LIST);
  ASTNode val = ParseValue(i);
  list.children.push_back(val);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode val = ParseValue(i);
    list.children.push_back(val);
  }
  return list;
}

ASTNode Parser::ParseAssignmentList(size_t &i) {
  std::cout << "Parsing AssignmentList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::ASSIGNMENT_LIST);
  ASTNode assignment = ParseAssignment(i);
  list.children.push_back(assignment);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode assignment = ParseAssignment(i);
    list.children.push_back(assignment);
  }
  return list;
}

ASTNode Parser::ParseAlterActionList(size_t &i) {
  std::cout << "Parsing AlterActionList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::ALTER_ACTION_LIST);
  ASTNode alterAction = ParseAlterAction(i);
  list.children.push_back(alterAction);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode alterAction = ParseAlterAction(i);
    list.children.push_back(alterAction);
  }
  return list;
}

ASTNode Parser::ParseColumnRef(size_t &i) {
  std::cout << "Parsing ColumnRef from: " << tokens[i] << "\n";
  ASTNode ref(NodeType::COLUMN_REF);
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i], "ParseColumnRef");
  if (tokens[i + 1].type == tokenizer::TokenType::PUNC_PERIOD) {
    ASTNode ref(NodeType::COLUMN_REF);
    ref.children.push_back(ASTNode(NodeType::TABLE_REF, tokens[i].value));
    i += 2;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i], "ParseColumnRef");
    ref.children.push_back(ASTNode(NodeType::COLUMN_REF, tokens[i].value));
    i++;
    return ref;
  } else {
    ASTNode ref(NodeType::COLUMN_REF, tokens[i].value);
    i++;
    return ref;
  }
}

ASTNode Parser::ParseTableRef(size_t &i) {
  std::cout << "Parsing TableRef from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i], "ParseTableRef");
  ASTNode ref(NodeType::TABLE_REF, tokens[i].value);
  i++;
  if (tokens[i].type == tokenizer::TokenType::KEYWORD_AS) {
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i], "ParseTableRef");
    ref.children.push_back(ASTNode(NodeType::ALIAS, tokens[i].value));
    i++;
  }
  return ref;
}

ASTNode Parser::ParseLogicalExpression(size_t &i) {
  std::cout << "Parsing LogicalExpression from: " << tokens[i] << "\n";
  ASTNode pre = ParseComparisonExpression(i);
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_AND: {
    ASTNode cond(NodeType::BINARY_OPERATION);
    cond.children.push_back(pre);
    ASTNode op(NodeType::BIN_OP_AND);
    cond.children.push_back(op);
    i++;
    ASTNode post = ParseLogicalExpression(i);
    cond.children.push_back(post);
    return cond;
  }
  case tokenizer::TokenType::OP_OR: {
    ASTNode cond(NodeType::BINARY_OPERATION);
    cond.children.push_back(pre);
    ASTNode op(NodeType::BIN_OP_OR);
    cond.children.push_back(op);
    i++;
    ASTNode post = ParseLogicalExpression(i);
    cond.children.push_back(post);
    return cond;
  }
  default:
    return pre;
  }
}

ASTNode Parser::ParseOrderList(size_t &i) {
  std::cout << "Parsing OrderList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::ORDER_LIST);
  ASTNode item = ParseOrderItem(i);
  list.children.push_back(item);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode item = ParseOrderItem(i);
    list.children.push_back(item);
  }
  return list;
}

ASTNode Parser::ParseColumnDef(size_t &i) {
  std::cout << "Parsing ColumnDef from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i], "ParseColumnDef");
  ASTNode def(NodeType::COLUMN_DEFINITION);
  def.children.push_back(ASTNode(NodeType::COLUMN_REF, tokens[i].value));
  i++;
  ASTNode type = ParseDataType(i);
  def.children.push_back(type);
  if (tokens[i].type != tokenizer::TokenType::PUNC_COMMA &&
      tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
    ASTNode constraints = ParseColumnConstraintsList(i);
    def.children.push_back(constraints);
  }
  return def;
}

ASTNode Parser::ParseValue(size_t &i) {
  std::cout << "Parsing Value from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::LITERAL_INT: {
    ASTNode val(NodeType::LITERAL_INT, tokens[i].value);
    i++;
    return val;
  }
  case tokenizer::TokenType::LITERAL_FLOAT: {
    ASTNode val(NodeType::LITERAL_FLOAT, tokens[i].value);
    i++;
    return val;
  }
  case tokenizer::TokenType::LITERAL_BOOL: {
    ASTNode val(NodeType::LITERAL_BOOL, tokens[i].value);
    i++;
    return val;
  }
  case tokenizer::TokenType::LITERAL_STRING: {
    ASTNode val(NodeType::LITERAL_STRING, tokens[i].value);
    i++;
    return val;
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseValue");
  }
}

ASTNode Parser::ParseAssignment(size_t &i) {
  std::cout << "Parsing Assignment from: " << tokens[i] << "\n";
  ASTNode assignment(NodeType::ASSIGNMENT);
  ASTNode colRef = ParseColumnRef(i);
  assignment.children.push_back(colRef);
  if (tokens[i].type != tokenizer::TokenType::OP_EQUAL)
    throw UnexpectedToken(tokens[i], "ParseAssignment");
  i++;
  ASTNode val = ParseValue(i);
  assignment.children.push_back(val);
  return assignment;
}

ASTNode Parser::ParseAlterAction(size_t &i) {
  std::cout << "Parsing AlterAction from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::KEYWORD_ADD: {
    ASTNode action(NodeType::ALTER_ACTION_ADD);
    i++;
    ASTNode addDef = ParseAddDefinition(i);
    action.children.push_back(addDef);
    return action;
  }
  case tokenizer::TokenType::KEYWORD_DROP: {
    ASTNode action(NodeType::ALTER_ACTION_DROP_COLUMN);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_COLUMN)
      throw UnexpectedToken(tokens[i], "ParseAlterAction");
    ASTNode colRef = ParseColumnRef(i);
    action.children.push_back(colRef);
    return action;
  }
  case tokenizer::TokenType::KEYWORD_MODIFY: {
    ASTNode action(NodeType::ALTER_ACTION_MODIFY_COLUMN);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_COLUMN)
      throw UnexpectedToken(tokens[i], "ParseAlterAction");
    i++;
    ASTNode type = ParseDataType(i);
    action.children.push_back(type);
    return action;
  }
  case tokenizer::TokenType::KEYWORD_RENAME: {
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TO: {
      ASTNode action(NodeType::ALTER_ACTION_RENAME);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i], "ParseAlterAction");
      ASTNode identifier(NodeType::IDENTIFIER, tokens[i].value);
      i++;
      action.children.push_back(identifier);
      return action;
    }
    case tokenizer::TokenType::KEYWORD_COLUMN: {
      ASTNode action(NodeType::ALTER_ACTION_RENAME_COLUMN);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i], "ParseAlterAction");
      ASTNode from(NodeType::IDENTIFIER, tokens[i].value);
      action.children.push_back(from);
      i++;
      if (tokens[i].type != tokenizer::TokenType::KEYWORD_TO)
        throw UnexpectedToken(tokens[i], "ParseAlterAction");
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i], "ParseAlterAction");
      ASTNode to(NodeType::IDENTIFIER, tokens[i].value);
      action.children.push_back(to);
      return action;
    }
    default:
      throw UnexpectedToken(tokens[i], "ParseAlterAction");
    }
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseAlterAction");
  }
}

ASTNode Parser::ParseOrderItem(size_t &i) {
  std::cout << "Parsing OrderItem from: " << tokens[i] << "\n";
  ASTNode orderItem(NodeType::ORDER_ITEM);
  ASTNode ref = ParseColumnRef(i);
  orderItem.children.push_back(ref);
  switch (tokens[i].type) {
  case tokenizer::TokenType::KEYWORD_ASC: {
    orderItem.children.push_back(ASTNode(NodeType::ORDER_ASC));
    break;
  }
  case tokenizer::TokenType::KEYWORD_DESC: {
    orderItem.children.push_back(ASTNode(NodeType::ORDER_DESC));
    break;
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseOrderItem");
  }
  i++;
  return orderItem;
}

ASTNode Parser::ParseColumnConstraintsList(size_t &i) {
  std::cout << "Parsing ColumnConstrainList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::COLUMN_CONSTRAINT_LIST);
  ASTNode constraint = ParseColumnConstraint(i);
  list.children.push_back(constraint);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode constraint = ParseColumnConstraint(i);
    list.children.push_back(constraint);
  }
  return list;
}

ASTNode Parser::ParseTableElementsList(size_t &i) {
  std::cout << "Parsing TableElementsList from: " << tokens[i] << "\n";
  ASTNode list(NodeType::TABLE_ELEMENT_LIST);
  ASTNode element = ParseTableElement(i);
  list.children.push_back(element);
  while (tokens[i].type == tokenizer::TokenType::PUNC_COMMA) {
    i++;
    ASTNode element = ParseTableElement(i);
    list.children.push_back(element);
  }
  return list;
}

ASTNode Parser::ParseAddDefinition(size_t &i) {
  std::cout << "Parsing AddDefinition from: " << tokens[i] << "\n";
  ASTNode def(NodeType::ADD_DEFINITION);
  switch (tokens[i].type) {
  case tokenizer::TokenType::KEYWORD_COLUMN: {
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i], "ParseAddDefinition");
    def.children.push_back(ASTNode(NodeType::IDENTIFIER, tokens[i].value));
    i++;
    ASTNode type = ParseDataType(i);
    def.children.push_back(type);
  }
  case tokenizer::TokenType::KEYWORD_CONSTRAINT: {
    ASTNode constraint = ParseConstraintDef(i);
    return constraint;
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseAddDefinition");
  }
  return def;
}

ASTNode Parser::ParseConstraintDef(size_t &i) {
  std::cout << "Parsing ConstraintDef from: " << tokens[i] << "\n";
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_CONSTRAINT)
    throw UnexpectedToken(tokens[i], "ParseConstraintDef");
  ASTNode constraintDef(NodeType::CONSTRAINT);
  i++;
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i], "ParseConstraintDef");
  ASTNode constraintName(NodeType::CONSTRAINT_NAME, tokens[i].value);
  constraintDef.children.push_back(constraintName);
  i++;
  ASTNode constraintType = ParseConstraintType(i);
  constraintDef.children.push_back(constraintType);
  return constraintDef;
}

ASTNode Parser::ParseDataType(size_t &i) {
  std::cout << "Parsing DataType from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::TYPE_INT: {
    i++;
    return ASTNode(NodeType::TYPE_INT);
  }
  case tokenizer::TokenType::TYPE_FLOAT: {
    i++;
    return ASTNode(NodeType::TYPE_FLOAT);
  }
  case tokenizer::TokenType::TYPE_TEXT: {
    i++;
    return ASTNode(NodeType::TYPE_TEXT);
  }
  case tokenizer::TokenType::TYPE_BOOLEAN: {
    i++;
    return ASTNode(NodeType::TYPE_BOOLEAN);
  }
  case tokenizer::TokenType::TYPE_BLOB: {
    i++;
    return ASTNode(NodeType::TYPE_BLOB);
  }
  case tokenizer::TokenType::TYPE_VARCHAR: {
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN)
      throw UnexpectedToken(tokens[i], "ParseDataType");
    i++;
    if (tokens[i].type != tokenizer::TokenType::LITERAL_INT)
      throw UnexpectedToken(tokens[i], "ParseDataType");
    ASTNode size(NodeType::LITERAL_INT, tokens[i].value);
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE)
      throw UnexpectedToken(tokens[i], "ParseDataType");
    i++;
    ASTNode varchar(NodeType::TYPE_VARCHAR);
    varchar.children.push_back(size);
    return varchar;
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseDataType");
  }
}

ASTNode Parser::ParseComparisonExpression(size_t &i) {
  std::cout << "Parsing ComparisionExpression from: " << tokens[i] << "\n";
  ASTNode pre = ParseAdditiveExpression(i);
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_EQUAL: {
    ASTNode compexp(NodeType::BINARY_OPERATION);
    compexp.children.push_back(pre);
    i++;
    compexp.children.push_back(NodeType::BIN_OP_EQUAL);
    ASTNode post = ParseAdditiveExpression(i);
    compexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_NOT_EQUAL: {
    ASTNode compexp(NodeType::BINARY_OPERATION);
    compexp.children.push_back(pre);
    i++;
    compexp.children.push_back(NodeType::BIN_OP_NOT_EQUAL);
    ASTNode post = ParseAdditiveExpression(i);
    compexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_LESS: {
    ASTNode compexp(NodeType::BINARY_OPERATION);
    compexp.children.push_back(pre);
    i++;
    compexp.children.push_back(NodeType::BIN_OP_LESS);
    ASTNode post = ParseAdditiveExpression(i);
    compexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_LESS_EQUAL: {
    ASTNode compexp(NodeType::BINARY_OPERATION);
    compexp.children.push_back(pre);
    i++;
    compexp.children.push_back(NodeType::BIN_OP_LESS_EQUAL);
    ASTNode post = ParseAdditiveExpression(i);
    compexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_GREATER: {
    ASTNode compexp(NodeType::BINARY_OPERATION);
    compexp.children.push_back(pre);
    i++;
    compexp.children.push_back(NodeType::BIN_OP_GREATER);
    ASTNode post = ParseAdditiveExpression(i);
    compexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_GREATER_EQUAL: {
    ASTNode compexp(NodeType::BINARY_OPERATION);
    compexp.children.push_back(pre);
    i++;
    compexp.children.push_back(NodeType::BIN_OP_GREATER_EQUAL);
    ASTNode post = ParseAdditiveExpression(i);
    compexp.children.push_back(post);
  }
  default:
    return pre;
  }
}

ASTNode Parser::ParseAdditiveExpression(size_t &i) {
  std::cout << "Parsing AdditiveExpression from: " << tokens[i] << "\n";
  ASTNode pre = ParseMultiplicativeExpression(i);
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_ADDITION: {
    ASTNode addexp(NodeType::BINARY_OPERATION);
    addexp.children.push_back(pre);
    i++;
    addexp.children.push_back(NodeType::BIN_OP_ADDITION);
    ASTNode post = ParseMultiplicativeExpression(i);
    addexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_SUBTRACT: {
    ASTNode addexp(NodeType::BINARY_OPERATION);
    addexp.children.push_back(pre);
    i++;
    addexp.children.push_back(NodeType::BIN_OP_SUBTRACT);
    ASTNode post = ParseMultiplicativeExpression(i);
    addexp.children.push_back(post);
  }
  default:
    return pre;
  }
}

ASTNode Parser::ParseMultiplicativeExpression(size_t &i) {
  std::cout << "Parsing MultiplicativeExpression from: " << tokens[i] << "\n";
  ASTNode pre = ParseUnaryExpression(i);
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_MULTIPLY: {
    ASTNode multexp(NodeType::BINARY_OPERATION);
    multexp.children.push_back(pre);
    i++;
    multexp.children.push_back(NodeType::BIN_OP_MULTIPLY);
    ASTNode post = ParseMultiplicativeExpression(i);
    multexp.children.push_back(post);
  }
  case tokenizer::TokenType::OP_DIVIDE: {
    ASTNode multexp(NodeType::BINARY_OPERATION);
    multexp.children.push_back(pre);
    i++;
    multexp.children.push_back(NodeType::BIN_OP_DIVIDE);
    ASTNode post = ParseMultiplicativeExpression(i);
    multexp.children.push_back(post);
  }
  default:
    return pre;
  }
}

ASTNode Parser::ParseUnaryExpression(size_t &i) {
  std::cout << "Parsing UnaryExpression from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_ADDITION: {
    i++;
    ASTNode unexp(NodeType::UNARY_OPERATION);
    unexp.children.push_back(NodeType::UN_OP_POS);
    ASTNode term = ParseTerm(i);
    unexp.children.push_back(term);
    return unexp;
  }
  case tokenizer::TokenType::OP_SUBTRACT: {
    i++;
    ASTNode unexp(NodeType::UNARY_OPERATION);
    unexp.children.push_back(NodeType::UN_OP_SUBTRACT);
    ASTNode term = ParseTerm(i);
    unexp.children.push_back(term);
    return unexp;
  }
  case tokenizer::TokenType::OP_NOT: {
    i++;
    ASTNode unexp(NodeType::UNARY_OPERATION);
    unexp.children.push_back(NodeType::UN_OP_NOT);
    ASTNode term = ParseTerm(i);
    unexp.children.push_back(term);
    return unexp;
  }
  default:
    return ParseTerm(i);
  }
}

ASTNode Parser::ParseTerm(size_t &i) {
  std::cout << "Parsing Term from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::LITERAL_INT: {
    ASTNode lit(NodeType::LITERAL_INT, tokens[i].value);
    i++;
    return lit;
  }
  case tokenizer::TokenType::LITERAL_FLOAT: {
    ASTNode lit(NodeType::LITERAL_FLOAT, tokens[i].value);
    i++;
    return lit;
  }
  case tokenizer::TokenType::LITERAL_STRING: {
    ASTNode lit(NodeType::LITERAL_STRING, tokens[i].value);
    i++;
    return lit;
  }
  default:
    return ParseColumnRef(i);
  }
}

ASTNode Parser::ParseColumnConstraint(size_t &i) {
  std::cout << "Parsing ColumnContraint from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_NOT: {
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_NULL)
      throw UnexpectedToken(tokens[i], "ParseColumnConstraint");
    i++;
    return ASTNode(NodeType::CONSTRAINT_NOT_NULL);
  }
  case tokenizer::TokenType::KEYWORD_UNIQUE: {
    i++;
    return ASTNode(NodeType::CONSTRAINT_UNIQUE);
  }
  case tokenizer::TokenType::KEYWORD_PRIMARY: {
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_KEY)
      throw UnexpectedToken(tokens[i], "ParseColumnConstraint");
    i++;
    return ASTNode(NodeType::CONSTRAINT_PRIMARY_KEY);
  }
  case tokenizer::TokenType::KEYWORD_DEFAULT: {
    i++;
    ASTNode constraint(NodeType::CONSTRAINT_DEFAULT);
    ASTNode val = ParseValue(i);
    constraint.children.push_back(val);
    return constraint;
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseColumnConstraint");
  }
}

ASTNode Parser::ParseTableElement(size_t &i) {
  std::cout << "Parsing TableElement from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::IDENTIFIER: {
    return ParseColumnDef(i);
  }
  case tokenizer::TokenType::KEYWORD_CONSTRAINT: {
    i++;
    return ParseColumnConstraint(i);
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseTableElement");
  }
}

ASTNode Parser::ParseConstraintType(size_t &i) {
  std::cout << "Parsing ConstraintType from: " << tokens[i] << "\n";
  switch (tokens[i].type) {
  case tokenizer::TokenType::KEYWORD_PRIMARY: {
    ASTNode constraintType(NodeType::CONSTRAINT_PRIMARY_KEY);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_KEY)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    ASTNode colList = ParseColumnList(i);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    constraintType.children.push_back(colList);
    return constraintType;
  }
  case tokenizer::TokenType::KEYWORD_FOREIGN: {
    ASTNode constraintType(NodeType::CONSTRAINT_FOREIGN_KEY);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_KEY)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    ASTNode colList = ParseColumnList(i);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    constraintType.children.push_back(colList);
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_REFERENCES)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    ASTNode tableRef = ParseTableRef(i);
    constraintType.children.push_back(tableRef);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    ASTNode refList = ParseColumnList(i);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    constraintType.children.push_back(refList);
    return constraintType;
  }
  case tokenizer::TokenType::KEYWORD_UNIQUE: {
    ASTNode constraintType(NodeType::CONSTRAINT_UNIQUE);
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    ASTNode colList = ParseColumnList(i);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE)
      throw UnexpectedToken(tokens[i], "ParseConstraintType");
    i++;
    constraintType.children.push_back(colList);
    return constraintType;
  }
  default:
    throw UnexpectedToken(tokens[i], "ParseConstraintType");
  }
}

} // namespace parser
