#include "nodes.h"

#include <sstream>
#include <string>
#include <vector>

namespace parser {

std::string NodeTypeToString(NodeType type) {
  switch (type) {
  case NodeType::STATEMENT_CREATE_TABLE:
    return "STATEMENT_ALTER_TABLE";
  case NodeType::STATEMENT_CREATE_DATABASE:
    return "STATEMENT_CREATE_DATABASE";
  case NodeType::STATEMENT_ALTER_TABLE:
    return "STATEMENT_ALTER_TABLE";
  case NodeType::STATEMENT_DROP_TABLE:
    return "STATEMENT_DROP_TABLE";
  case NodeType::STATEMENT_DROP_DATABASE:
    return "STATEMENT_DROP_DATABASE";
  case NodeType::STATEMENT_DROP_COLUMN:
    return "STATEMENT_DROP_COLUMN";
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
  case NodeType::CLAUSE_GROUP_BY:
    return "CLAUSE_GROUP_BY";
  case NodeType::CLAUSE_ORDER_BY:
    return "CLAUSE_ORDER_BY";
  case NodeType::ORDER_LIST:
    return "ORDER_LIST";
  case NodeType::ORDER_ITEM:
    return "ORDER_ITEM";
  case NodeType::ORDER_ASC:
    return "ORDER_ASC";
  case NodeType::ORDER_DESC:
    return "ORDER_DESC";
  case NodeType::CLAUSE_LIMIT:
    return "CLAUSE_LIMIT";
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
  case NodeType::ALIAS:
    return "ALIAS";
  case NodeType::TABLE_LIST:
    return "TABLE_LIST";
  case NodeType::TABLE_ELEMENT_LIST:
    return "TABLE_ELEMENT_LIST";
  case NodeType::COLUMN_DEFINITION_LIST:
    return "COLUMN_DEFINITION_LIST";
  case NodeType::COLUMN_DEFINITION:
    return "COLUMN_DEFINITION";
  case NodeType::COLUMN_CONSTRAINT_LIST:
    return "COLUMN_CONSTRAINT_LIST";
  case NodeType::COLUMN_CONSTRAINT:
    return "COLUMN_CONSTRAINT";
  case NodeType::COLUMN_LIST:
    return "COLUMN_LIST";
  case NodeType::COLUMN_REF:
    return "COLUMN_REF";
  case NodeType::COLUMNS_ALL:
    return "COLUMNS_ALL";
  case NodeType::DATABASE_REF:
    return "DATABASE_REF";
  case NodeType::ALTER_ACTION_LIST:
    return "ALTER_ACTION_LIST";
  case NodeType::SELECT_LIST:
    return "SELECT_LIST";
  case NodeType::NULL_TYPE:
    return "NULL_TYPE";
  case NodeType::LITERAL_INT:
    return "LITERAL_INT";
  case NodeType::LITERAL_STRING:
    return "LITERAL_STRING";
  case NodeType::LITERAL_FLOAT:
    return "LITERAL_FLOAT";
  case NodeType::LITERAL_BOOL:
    return "LITERAL_BOOL";
  case NodeType::TYPE_INT:
    return "TYPE_INT";
  case NodeType::TYPE_TEXT:
    return "TYPE_TEXT";
  case NodeType::TYPE_FLOAT:
    return "TYPE_FLOAT";
  case NodeType::TYPE_BOOLEAN:
    return "TYPE_BOOLEAN";
  case NodeType::TYPE_BLOB:
    return "TYPE_BLOB";
  case NodeType::TYPE_VARCHAR:
    return "TYPE_VARCHAR";
  case NodeType::VALUE_LIST:
    return "VALUE_LIST";
  case NodeType::ASSIGNMENT_LIST:
    return "ASSIGNMENT_LIST";
  case NodeType::ASSIGNMENT:
    return "ASSIGNMENT";
  case NodeType::CONDITION:
    return "CONDITION";
  case NodeType::CONSTRAINT:
    return "CONSTRAINT";
  case NodeType::CONSTRAINT_NAME:
    return "CONSTRAINT_NAME";
  case NodeType::CONSTRAINT_NOT_NULL:
    return "CONSTRAINT_NOT_NULL";
  case NodeType::CONSTRAINT_UNIQUE:
    return "CONSTRAINT_UNIQUE";
  case NodeType::CONSTRAINT_PRIMARY_KEY:
    return "CONSTRAINT_PRIMARY_KEY";
  case NodeType::CONSTRAINT_FOREIGN_KEY:
    return "CONSTRAINT_FOREIGN_KEY";
  case NodeType::CONSTRAINT_DEFAULT:
    return "CONSTRAINT_DEFAULT";
  case NodeType::BIN_OP_SUBTRACT:
    return "BIN_OP_SUBTRACT";
  case NodeType::BIN_OP_DIVIDE:
    return "BIN_OP_DIVIDE";
  case NodeType::BIN_OP_MULTIPLY:
    return "BIN_OP_MULTIPLY";
  case NodeType::BIN_OP_ADDITION:
    return "BIN_OP_ADDITION";
  case NodeType::BIN_OP_OR:
    return "BIN_OP_OR";
  case NodeType::BIN_OP_AND:
    return "BIN_OP_AND";
  case NodeType::BIN_OP_LIKE:
    return "BIN_OP_LIKE";
  case NodeType::BIN_OP_GREATER:
    return "BIN_OP_GREATER";
  case NodeType::BIN_OP_GREATER_EQUAL:
    return "BIN_OP_GREATER_EQUAL";
  case NodeType::BIN_OP_LESS:
    return "BIN_OP_LESS";
  case NodeType::BIN_OP_LESS_EQUAL:
    return "BIN_OP_LESS_EQUAL";
  case NodeType::BIN_OP_EQUAL:
    return "BIN_OP_EQUAL";
  case NodeType::BIN_OP_NOT_EQUAL:
    return "BIN_OP_NOT_EQUAL";
  case NodeType::UN_OP_POS:
    return "UN_OP_POS";
  case NodeType::UN_OP_SUBTRACT:
    return "UN_OP_SUBTRACT";
  case NodeType::UN_OP_NOT:
    return "UN_OP_NOT";
  case NodeType::ALTER_ACTION_ADD:
    return "ALTER_ACTION_ADD";
  case NodeType::ALTER_ACTION_DROP_COLUMN:
    return "ALTER_ACTION_DROP_COLUMN";
  case NodeType::ALTER_ACTION_RENAME_COLUMN:
    return "ALTER_ACTION_RENAME_COLUMN";
  case NodeType::ALTER_ACTION_MODIFY_COLUMN:
    return "ALTER_ACTION_MODIFY_COLUMN";
  case NodeType::ALTER_ACTION_RENAME:
    return "ALTER_ACTION_RENAME";
  case NodeType::ADD_DEFINITION:
    return "ADD_DEFINITION";
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
