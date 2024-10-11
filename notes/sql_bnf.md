<sql> ::= <select_stmt> 
        | <insert_stmt> 
        | <update_stmt> 
        | <delete_stmt> 
        | <create_stmt>

<select_stmt> ::= "SELECT" <select_expr_list> "FROM" <table_list> [<where_clause>] [<group_by_clause>] [<order_by_clause>] [<limit_clause>]

<select_expr_list> ::= "*" 
                     | <select_expr> { "," <select_expr> }*

<select_expr> ::= <column_name> 
                | <column_name> "AS" <alias>

<table_list> ::= <table_name> { "," <table_name> }*

<where_clause> ::= "WHERE" <condition>

<group_by_clause> ::= "GROUP BY" <column_list>

<order_by_clause> ::= "ORDER BY" <column_list> [ "ASC" | "DESC" ]

<limit_clause> ::= "LIMIT" <number>

<insert_stmt> ::= "INSERT INTO" <table_name> "(" <column_list> ")" "VALUES" "(" <value_list> ")"

<update_stmt> ::= "UPDATE" <table_name> "SET" <assignment_list> [<where_clause>]

<assignment_list> ::= <assignment> { "," <assignment> }*

<assignment> ::= <column_name> "=" <value>

<delete_stmt> ::= "DELETE FROM" <table_name> [<where_clause>]

<create_stmt> ::= "CREATE TABLE" <table_name> "(" <column_def_list> ")"

<column_def_list> ::= <column_def> { "," <column_def> }*

<column_def> ::= <column_name> <data_type> [<column_constraint>]

<column_constraint> ::= "PRIMARY KEY"
                      | "NOT NULL"
                      | "UNIQUE"
                      | "DEFAULT" <default_value>

<condition> ::= <expression> <comparison_op> <expression>
             | <expression> "IS" "NULL"
             | <expression> "IN" "(" <select_stmt> ")"
             | <expression> "BETWEEN" <expression> "AND" <expression>
             | <expression> "LIKE" <pattern>

<expression> ::= <column_name>
               | <literal>
               | <function_call>

<comparison_op> ::= "=" | "<>" | ">" | "<" | ">=" | "<="

<function_call> ::= <function_name> "(" [<expression_list>] ")"

<expression_list> ::= <expression> { "," <expression> }*

<column_list> ::= <column_name> { "," <column_name> }*

<value_list> ::= <value> { "," <value> }*

<value> ::= <literal> | "NULL"

<literal> ::= <number> | <string> | <boolean>

<number> ::= <integer> | <decimal>

<string> ::= "'" <character> {<character>}* "'"

<boolean> ::= "TRUE" | "FALSE"

<table_name> ::= <identifier>

<column_name> ::= <identifier>

<alias> ::= <identifier>

<function_name> ::= <identifier>

<identifier> ::= <letter> {<letter> | <digit> | "_" }*

<data_type> ::= "INT" 
             | "VARCHAR" "(" <number> ")" 
             | "TEXT" 
             | "BOOLEAN" 
             | "DATE"

