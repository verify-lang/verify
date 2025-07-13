#ifndef AST_H
#define AST_H

#include "types.h"

typedef enum
{
  AST_PROGRAM,
  AST_FUNCTION,
  AST_VAR_DECL,
  AST_BLOCK,
  AST_RETURN,
  AST_IF,
  AST_WHILE,
  AST_BINARY_OP,
  AST_UNARY_OP,
  AST_CALL,
  AST_IDENTIFIER,
  AST_LITERAL_INT,
  AST_LITERAL_BOOL,
  AST_LITERAL_STRING,
  AST_ASSIGNMENT,
  AST_STRUCT_DECL,
  AST_FIELD_ACCESS
} ast_node_type_t;

typedef enum
{
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_EQ,
  OP_NE,
  OP_LT,
  OP_LE,
  OP_GT,
  OP_GE,
  OP_AND,
  OP_OR,
  OP_NOT,
  OP_NEG
} operator_t;

typedef struct ast_node
{
  ast_node_type_t type;
  type_t* data_type;
  union
  {
    struct
    {
      struct ast_node** statements;
      int statement_count;
    } program;

    struct
    {
      char* name;
      parameter_t* params;
      int param_count;
      type_t* return_type;
      struct ast_node* body;
    } function;

    struct
    {
      char* name;
      type_t* type;
      struct ast_node* initializer;
      int is_mutable;
    } var_decl;

    struct
    {
      struct ast_node** statements;
      int statement_count;
    } block;

    struct
    {
      struct ast_node* expression;
    } return_stmt;

    struct
    {
      struct ast_node* condition;
      struct ast_node* then_stmt;
      struct ast_node* else_stmt;
    } if_stmt;

    struct
    {
      struct ast_node* condition;
      struct ast_node* body;
    } while_stmt;

    struct
    {
      operator_t op;
      struct ast_node* left;
      struct ast_node* right;
    } binary_op;

    struct
    {
      operator_t op;
      struct ast_node* operand;
    } unary_op;

    struct
    {
      char* name;
      struct ast_node** args;
      int arg_count;
    } call;

    struct
    {
      char* name;
    } identifier;

    struct
    {
      int value;
    } literal_int;

    struct
    {
      int value;
    } literal_bool;

    struct
    {
      char* value;
    } literal_string;

    struct
    {
      struct ast_node* target;
      struct ast_node* value;
    } assignment;

    struct
    {
      char* name;
      field_t** fields;
      int field_count;
      method_t** methods;
      int method_count;
    } struct_decl;

    struct
    {
      struct ast_node* object;
      char* field_name;
    } field_access;
  } data;
} ast_node_t;

ast_node_t* ast_create_program(ast_node_t** statements, int count);
ast_node_t* ast_create_function(char* name, parameter_t* params,
                                int param_count, type_t* return_type,
                                ast_node_t* body);
ast_node_t* ast_create_var_decl(char* name, type_t* type,
                                ast_node_t* initializer, int is_mutable);
ast_node_t* ast_create_block(ast_node_t** statements, int count);
ast_node_t* ast_create_return(ast_node_t* expression);
ast_node_t* ast_create_if(ast_node_t* condition, ast_node_t* then_stmt,
                          ast_node_t* else_stmt);
ast_node_t* ast_create_while(ast_node_t* condition, ast_node_t* body);
ast_node_t* ast_create_binary_op(operator_t op, ast_node_t* left,
                                 ast_node_t* right);
ast_node_t* ast_create_unary_op(operator_t op, ast_node_t* operand);
ast_node_t* ast_create_call(char* name, ast_node_t** args, int arg_count);
ast_node_t* ast_create_identifier(char* name);
ast_node_t* ast_create_literal_int(int value);
ast_node_t* ast_create_literal_bool(int value);
ast_node_t* ast_create_literal_string(char* value);
ast_node_t* ast_create_assignment(ast_node_t* target, ast_node_t* value);

ast_node_t* ast_create_struct_decl(char* name, field_t** fields,
                                   int field_count, method_t** methods,
                                   int method_count);
ast_node_t* ast_create_field_access(ast_node_t* object, char* field_name);

void ast_free(ast_node_t* node);
void ast_print(ast_node_t* node, int indent);

#endif
