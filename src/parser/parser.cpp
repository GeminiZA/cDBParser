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

Parser::Parser(std::vector<tokenizer::Token> tokens) : tokens(tokens) {};

std::optional<ASTNode> Parser::GetRoot() { return root; }

void Parser::TryParse() {
  size_t i = 0;
  while (i < tokens.size()) {
    try {
      ParseStatement(i);
    } catch (std::exception &e) {
      std::cerr << "Error Parsing statement: " << e.what();
      return;
    }
  }
}

ASTNode Parser::ParseStatement(size_t &i) {
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
      throw UnexpectedToken(tokens[i]);
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
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_INSERT: { // insert_statement
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_INTO) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    statement = ASTNode(NodeType::STATEMENT_INSERT);
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
      throw UnexpectedToken(tokens[i]);
    }
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    ASTNode columnList = ParseColumnList(i);
    statement.children.push_back(columnList);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_VALUES) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    ASTNode valueList = ParseValueList(i);
    statement.children.push_back(valueList);
    if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_UPDATE: { // update_statement
    i++;
    statement = ASTNode(NodeType::STATEMENT_UPDATE);
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
      throw UnexpectedToken(tokens[i]);
    }
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_SET) {
      throw UnexpectedToken(tokens[i]);
    }
    i++;
    ASTNode AssignmentList = ParseAssignmentList(i);
    statement.children.push_back(AssignmentList);
    // WHERE CLAUSE
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) {
      ASTNode clause = ParseWhereClause(i);
      statement.children.push_back(clause);
    }
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_CREATE: { // create_statement
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TABLE: {
      statement = ASTNode(NodeType::STATEMENT_CREATE_TABLE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER) {
        throw UnexpectedToken(tokens[i]);
      }
      ASTNode tableRef = ParseTableRef(i);
      statement.children.push_back(tableRef);
      if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_OPEN) {
        throw UnexpectedToken(tokens[i]);
      }
      i++;
      ASTNode tableElementsList = ParseTableElementsList(i);
      statement.children.push_back(tableElementsList);
      if (tokens[i].type != tokenizer::TokenType::PUNC_PAREN_CLOSE) {
        throw UnexpectedToken(tokens[i]);
      }
      i++;
      return statement;
    }
    case tokenizer::TokenType::KEYWORD_DATABASE: {
      throw UnexpectedToken(tokens[i]);
    }
    default:
      throw UnexpectedToken(tokens[i]);
    }
  }
  case tokenizer::TokenType::KEYWORD_DELETE: { // delete_statement
    statement = ASTNode(NodeType::STATEMENT_DELETE);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_FROM)
      throw UnexpectedToken(tokens[i]);
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i]);
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    if (tokens[i].type == tokenizer::TokenType::KEYWORD_WHERE) { // WHERE clause
      ASTNode clause = ParseWhereClause(i);
    }
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_ALTER: { // alter_statement
    statement = ASTNode(NodeType::STATEMENT_ALTER_TABLE);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_TABLE)
      throw UnexpectedToken(tokens[i]);
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i]);
    i++;
    ASTNode tableRef = ParseTableRef(i);
    statement.children.push_back(tableRef);
    ASTNode alterActions = ParseAlterActionList(i);
    statement.children.push_back(alterActions);
    return statement;
  }
  case tokenizer::TokenType::KEYWORD_DROP: { // drop_statement
    i++;
    switch (tokens[i].type) {
    case tokenizer::TokenType::KEYWORD_TABLE: {
      statement = ASTNode(NodeType::STATEMENT_DROP_TABLE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i]);
      ASTNode tableRef = ParseTableRef(i);
      statement.children.push_back(tableRef);
      return statement;
    }
    case tokenizer::TokenType::KEYWORD_COLUMN: {
      statement = ASTNode(NodeType::STATEMENT_DROP_COLUMN);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i]);
      ASTNode columnRef = ParseColumnRef(i);
      statement.children.push_back(columnRef);
    }
    case tokenizer::TokenType::KEYWORD_DATABASE: {
      statement = ASTNode(NodeType::STATEMENT_DROP_DATABASE);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i]);
      ASTNode databaseRef(NodeType::DATABASE_REF, tokens[i].value);
      statement.children.push_back(databaseRef);
    }
    default:
      throw UnexpectedToken(tokens[i]);
    }
  }
  default:
    throw std::invalid_argument("Unexpected Token: " + tokens[i].ToString());
  }
  return statement;
}

ASTNode Parser::ParseSelectExpressionList(size_t &i) {
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
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_WHERE)
    throw UnexpectedToken(tokens[i]);
  ASTNode clause(NodeType::CLAUSE_WHERE);
  i++;
  ASTNode condition = ParseLogicalExpression(i);
  clause.children.push_back(condition);
  return clause;
}

ASTNode Parser::ParseGroupByClause(size_t &i) {
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_GROUP)
    throw UnexpectedToken(tokens[i]);
  i++;
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_BY)
    throw UnexpectedToken(tokens[i]);
  i++;
  ASTNode clause(NodeType::CLAUSE_GROUP_BY);
  ASTNode cols = ParseColumnList(i);
  clause.children.push_back(cols);
  return clause;
}

ASTNode Parser::ParseOrderByClause(size_t &i) {
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_ORDER)
    throw UnexpectedToken(tokens[i]);
  i++;
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_BY)
    throw UnexpectedToken(tokens[i]);
  i++;
  ASTNode clause(NodeType::CLAUSE_ORDER_BY);
  ASTNode orderList = ParseOrderList(i);
  clause.children.push_back(orderList);
  return clause;
}

ASTNode Parser::ParseLimitClause(size_t &i) {
  if (tokens[i].type != tokenizer::TokenType::KEYWORD_LIMIT)
    throw UnexpectedToken(tokens[i]);
  i++;
  ASTNode clause(NodeType::CLAUSE_LIMIT);
  if (tokens[i].type != tokenizer::TokenType::LITERAL_INT)
    throw UnexpectedToken(tokens[i]);
  clause.children.push_back(ASTNode(NodeType::LITERAL_INT, tokens[i].value));
  i++;
  return clause;
}

ASTNode Parser::ParseColumnList(size_t &i) {
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
  ASTNode ref(NodeType::COLUMN_REF);
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i]);
  if (tokens[i + 1].type == tokenizer::TokenType::PUNC_PERIOD) {
    ASTNode ref(NodeType::COLUMN_REF);
    ref.children.push_back(ASTNode(NodeType::TABLE_REF, tokens[i].value));
    i += 2;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i]);
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
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i]);
  ASTNode ref(NodeType::TABLE_REF, tokens[i].value);
  i++;
  if (tokens[i].type == tokenizer::TokenType::KEYWORD_AS) {
    i++;
    if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
      throw UnexpectedToken(tokens[i]);
    ref.children.push_back(ASTNode(NodeType::ALIAS, tokens[i].value));
    i++;
  }
  return ref;
}

ASTNode Parser::ParseLogicalExpression(size_t &i) {
  ASTNode cond(NodeType::BINARY_OPERATION);
  ASTNode pre = ParseComparisionExpression(i);
  cond.children.push_back(pre);
  switch (tokens[i].type) {
  case tokenizer::TokenType::OP_AND: {
    ASTNode op(NodeType::BIN_OP_AND);
    cond.children.push_back(op);
    i++;
    ASTNode post = ParseLogicalExpression(i);
    cond.children.push_back(post);
    return cond;
  }
  case tokenizer::TokenType::OP_OR: {
    ASTNode op(NodeType::BIN_OP_OR);
    cond.children.push_back(op);
    i++;
    ASTNode post = ParseLogicalExpression(i);
    cond.children.push_back(post);
    return cond;
  }
  default:
    return cond;
  }
}

ASTNode Parser::ParseOrderList(size_t &i) {
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
  if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
    throw UnexpectedToken(tokens[i]);
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
  switch (tokens[i].type) {
  case tokenizer::TokenType::LITERAL_INT: {
    ASTNode val(NodeType::LITERAL_INT, tokens[i].value);
    return val;
  }
  case tokenizer::TokenType::LITERAL_FLOAT: {
    ASTNode val(NodeType::LITERAL_FLOAT, tokens[i].value);
    return val;
  }
  case tokenizer::TokenType::LITERAL_BOOL: {
    ASTNode val(NodeType::LITERAL_BOOL, tokens[i].value);
    return val;
  }
  case tokenizer::TokenType::LITERAL_STRING: {
    ASTNode val(NodeType::LITERAL_STRING, tokens[i].value);
    return val;
  }
  default:
    throw UnexpectedToken(tokens[i]);
  }
}

ASTNode Parser::ParseAssignment(size_t &i) {
  ASTNode assignment(NodeType::ASSIGNMENT);
  ASTNode colRef = ParseColumnRef(i);
  assignment.children.push_back(colRef);
  if (tokens[i].type != tokenizer::TokenType::OP_EQUAL)
    throw UnexpectedToken(tokens[i]);
  i++;
  ASTNode val = ParseValue(i);
  assignment.children.push_back(val);
  return assignment;
}

ASTNode Parser::ParseAlterAction(size_t &i) {
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
      throw UnexpectedToken(tokens[i]);
    ASTNode colRef = ParseColumnRef(i);
    action.children.push_back(colRef);
    return action;
  }
  case tokenizer::TokenType::KEYWORD_MODIFY: {
    ASTNode action(NodeType::ALTER_ACTION_MODIFY_COLUMN);
    i++;
    if (tokens[i].type != tokenizer::TokenType::KEYWORD_COLUMN)
      throw UnexpectedToken(tokens[i]);
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
        throw UnexpectedToken(tokens[i]);
      ASTNode identifier(NodeType::IDENTIFIER, tokens[i].value);
      i++;
      action.children.push_back(identifier);
      return action;
    }
    case tokenizer::TokenType::KEYWORD_COLUMN: {
      ASTNode action(NodeType::ALTER_ACTION_RENAME_COLUMN);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i]);
      ASTNode from(NodeType::IDENTIFIER, tokens[i].value);
      action.children.push_back(from);
      i++;
      if (tokens[i].type != tokenizer::TokenType::KEYWORD_TO)
        throw UnexpectedToken(tokens[i]);
      i++;
      if (tokens[i].type != tokenizer::TokenType::IDENTIFIER)
        throw UnexpectedToken(tokens[i]);
      ASTNode to(NodeType::IDENTIFIER, tokens[i].value);
      action.children.push_back(to);
      return action;
    }
    default:
      throw UnexpectedToken(tokens[i]);
    }
  }
  default:
    throw UnexpectedToken(tokens[i]);
  }
}

ASTNode Parser::ParseOrderItem(size_t &i) {}

} // namespace parser
