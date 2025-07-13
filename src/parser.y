%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.h"

extern int yylex();
extern int yylineno;
extern char *yytext;

ast_node_t *root = NULL;

void yyerror(const char *msg) {
  fprintf(stderr, "parse error at line %d: %s\n", yylineno, msg);
}

int statement_count = 0;
int statement_capacity = 100;
ast_node_t **statements = NULL;

void add_statement(ast_node_t *stmt) {
  if (!statements) {
    statements = malloc(statement_capacity * sizeof(ast_node_t*));
  }

  if (statement_count >= statement_capacity) {
    statement_capacity *= 2;
    statements = realloc(statements, statement_capacity * sizeof(ast_node_t*));
  }

  statements[statement_count++] = stmt;
}

int arg_count = 0;
int arg_capacity = 10;
ast_node_t **args = NULL;

void add_arg(ast_node_t *arg) {
  if (!args) {
    args = malloc(arg_capacity * sizeof(ast_node_t*));
  }

  if (arg_count >= arg_capacity) {
    arg_capacity *= 2;
    args = realloc(args, arg_capacity * sizeof(ast_node_t*));
  }

  args[arg_count++] = arg;
}

int param_count = 0;
int param_capacity = 10;
parameter_t *params = NULL;

void add_param(parameter_t param) {
  if (!params) {
    params = malloc(param_capacity * sizeof(parameter_t));
  }

  if (param_count >= param_capacity) {
    param_capacity *= 2;
    params = realloc(params, param_capacity * sizeof(parameter_t));
  }

  params[param_count++] = param;
}

void reset_params() {
  param_count = 0;
}

void reset_args() {
  arg_count = 0;
}

void reset_statements() {
  statement_count = 0;
}

/*struct decl field*/
int field_count = 0;
int field_capacity = 10;
field_t **fields = NULL;

int method_count = 0;
int method_capacity = 10;
method_t **methods = NULL;

void add_field(field_t *field) {
  if (!fields) {
    fields = malloc(field_capacity * sizeof(field_t*));
  }

  if (field_count >= field_capacity) {
    field_capacity *= 2;
    fields = realloc(fields, field_capacity * sizeof(field_t*));
  }

  fields[field_count++] = field;
}

void add_method(method_t *method) {
  if (!methods) {
    methods = malloc(method_capacity * sizeof(method_t*));
  }

  if (method_count >= method_capacity) {
    method_capacity *= 2;
    methods = realloc(methods, method_capacity * sizeof(method_t*));
  }

  methods[method_count++] = method;
}

void reset_fields() {
  field_count = 0;
}

void reset_methods() {
  method_count = 0;
}
%}

%union {
  int int_val;
  int bool_val;
  char *string_val;
  ast_node_t *node;
  type_t *type;
  parameter_t param;
}

%token <int_val> INT_LITERAL
%token <bool_val> BOOL_LITERAL
%token <string_val> STRING_LITERAL IDENTIFIER
%token DEF LET CONST IF ELSE WHILE RETURN
%token INT_TYPE BOOL_TYPE STRING_TYPE
%token EQ NE LE GE AND OR ARROW
%token STRUCT SELF

%type <node> program statement_list statement function_def var_decl block
%type <node> expression primary_expr call_expr assignment_expr
%type <node> if_stmt while_stmt return_stmt
%type <type> type_spec
%type <param> parameter
%type <node> struct_decl field_access_expr

%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!' UNARY_MINUS
%left '.'
%left '(' ')'

%%

program:
  statement_list { 
    root = ast_create_program(statements, statement_count);
    reset_statements();
  }
  ;

statement_list:
  statement { add_statement($1); }
  | statement_list statement { add_statement($2); }
  ;

statement:
  function_def { $$ = $1; }
  | struct_decl { $$ = $1; }
  | var_decl ';' { $$ = $1; }
  | if_stmt { $$ = $1; }
  | while_stmt { $$ = $1; }
  | return_stmt ';' { $$ = $1; }
  | assignment_expr ';' { $$ = $1; }
  | expression ';' { $$ = $1; }
  ;

struct_decl:
  STRUCT IDENTIFIER '{' struct_body '}' {
    field_t **field_copy = malloc(field_count * sizeof(field_t*));
    memcpy(field_copy, fields, field_count * sizeof(field_t*));
    method_t **method_copy = malloc(method_count * sizeof(method_t*));
    memcpy(method_copy, methods, method_count * sizeof(method_t*));
    int f_count = field_count;
    int m_count = method_count;
    reset_fields();
    reset_methods();
    $$ = ast_create_struct_decl($2, field_copy, f_count, method_copy, m_count);
  }
  ;

struct_body:
  /* empty */
  | struct_members
  ;

struct_members:
  struct_member
  | struct_members struct_member
  ;

struct_member:
  field_decl
  | method_decl
  ;

field_decl:
  IDENTIFIER ':' type_spec ';' {
    field_t *field = field_create($1, $3);
    add_field(field);
  }
  ;

method_decl:
  DEF IDENTIFIER '(' SELF ')' ARROW type_spec block {
    parameter_t *self_param = malloc(sizeof(parameter_t));
    self_param->name = strdup("self");
    self_param->type = NULL;
    method_t *method = method_create($2, self_param, 1, $7, $8);
    add_method(method);
    reset_params();
  }
  | DEF IDENTIFIER '(' SELF ',' parameter_list ')' ARROW type_spec block {
    parameter_t *param_copy = malloc((param_count + 1) * sizeof(parameter_t));
    param_copy[0].name = strdup("self");
    param_copy[0].type = NULL;
    memcpy(&param_copy[1], params, param_count * sizeof(parameter_t));
    int count = param_count + 1;
    method_t *method = method_create($2, param_copy, count, $9, $10);
    add_method(method);
    reset_params();
  }
  | DEF IDENTIFIER '(' ')' ARROW type_spec block {
    method_t *method = method_create($2, NULL, 0, $6, $7);
    add_method(method);
    reset_params();
  }
  ;

function_def:
  DEF IDENTIFIER '(' ')' ARROW type_spec block {
    reset_params();
    $$ = ast_create_function($2, NULL, 0, $6, $7);
  }
  | DEF IDENTIFIER '(' parameter_list ')' ARROW type_spec block {
    parameter_t *param_copy = malloc(param_count * sizeof(parameter_t));
    memcpy(param_copy, params, param_count * sizeof(parameter_t));
    int count = param_count;
    reset_params();
    $$ = ast_create_function($2, param_copy, count, $7, $8);
  }
  ;

parameter_list:
  parameter { add_param($1); }
  | parameter_list ',' parameter { add_param($3); }
  ;

parameter:
  IDENTIFIER ':' type_spec { 
    $$.name = $1;
    $$.type = $3;
  }
  ;

var_decl:
  LET IDENTIFIER ':' type_spec '=' expression {
    $$ = ast_create_var_decl($2, $4, $6, 1);
  }
  | CONST IDENTIFIER ':' type_spec '=' expression {
    $$ = ast_create_var_decl($2, $4, $6, 0);
  }
  ;

type_spec:
  INT_TYPE { $$ = type_create_int(); }
  | BOOL_TYPE { $$ = type_create_bool(); }
  | STRING_TYPE { $$ = type_create_string(); }
  | IDENTIFIER { $$ = type_create_custom($1); }
  ;

block:
  '{' '}' { 
    $$ = ast_create_block(NULL, 0);
  }
  | '{' statement_list '}' {
    ast_node_t **stmt_copy = malloc(statement_count * sizeof(ast_node_t*));
    memcpy(stmt_copy, statements, statement_count * sizeof(ast_node_t*));
    int count = statement_count;
    reset_statements();
    $$ = ast_create_block(stmt_copy, count);
  }
  ;

if_stmt:
  IF '(' expression ')' block {
    $$ = ast_create_if($3, $5, NULL);
  }
  | IF '(' expression ')' block ELSE block {
    $$ = ast_create_if($3, $5, $7);
  }
  ;

while_stmt:
  WHILE '(' expression ')' block {
    $$ = ast_create_while($3, $5);
  }
  ;

return_stmt:
  RETURN { $$ = ast_create_return(NULL); }
  | RETURN expression { $$ = ast_create_return($2); }
  ;

assignment_expr:
  IDENTIFIER '=' expression {
    $$ = ast_create_assignment(ast_create_identifier($1), $3);
  }
  ;

expression:
  primary_expr { $$ = $1; }
  | field_access_expr { $$ = $1; }
  | expression '+' expression { $$ = ast_create_binary_op(OP_ADD, $1, $3); }
  | expression '-' expression { $$ = ast_create_binary_op(OP_SUB, $1, $3); }
  | expression '*' expression { $$ = ast_create_binary_op(OP_MUL, $1, $3); }
  | expression '/' expression { $$ = ast_create_binary_op(OP_DIV, $1, $3); }
  | expression '%' expression { $$ = ast_create_binary_op(OP_MOD, $1, $3); }
  | expression EQ expression { $$ = ast_create_binary_op(OP_EQ, $1, $3); }
  | expression NE expression { $$ = ast_create_binary_op(OP_NE, $1, $3); }
  | expression '<' expression { $$ = ast_create_binary_op(OP_LT, $1, $3); }
  | expression LE expression { $$ = ast_create_binary_op(OP_LE, $1, $3); }
  | expression '>' expression { $$ = ast_create_binary_op(OP_GT, $1, $3); }
  | expression GE expression { $$ = ast_create_binary_op(OP_GE, $1, $3); }
  | expression AND expression { $$ = ast_create_binary_op(OP_AND, $1, $3); }
  | expression OR expression { $$ = ast_create_binary_op(OP_OR, $1, $3); }
  | '!' expression { $$ = ast_create_unary_op(OP_NOT, $2); }
  | '-' expression %prec UNARY_MINUS { $$ = ast_create_unary_op(OP_NEG, $2); }
  | '(' expression ')' { $$ = $2; }
  | call_expr { $$ = $1; }
  ;

field_access_expr:
  expression '.' IDENTIFIER {
    $$ = ast_create_field_access($1, $3);
  }
  ;

call_expr:
  IDENTIFIER '(' ')' {
    reset_args();
    $$ = ast_create_call($1, NULL, 0);
  }
  | IDENTIFIER '(' arg_list ')' {
    ast_node_t **arg_copy = malloc(arg_count * sizeof(ast_node_t*));
    memcpy(arg_copy, args, arg_count * sizeof(ast_node_t*));
    int count = arg_count;
    reset_args();
    $$ = ast_create_call($1, arg_copy, count);
  }
  ;

arg_list:
  expression { add_arg($1); }
  | arg_list ',' expression { add_arg($3); }
  ;

primary_expr:
  INT_LITERAL { $$ = ast_create_literal_int($1); }
  | BOOL_LITERAL { $$ = ast_create_literal_bool($1); }
  | STRING_LITERAL { $$ = ast_create_literal_string($1); }
  | IDENTIFIER { $$ = ast_create_identifier($1); }
  | SELF { $$ = ast_create_identifier("self"); }
  ;

%%
