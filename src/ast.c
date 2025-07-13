#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

ast_node_t* ast_create_program(ast_node_t** statements, int count)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_PROGRAM;
  node->data_type = NULL;
  node->data.program.statements = statements;
  node->data.program.statement_count = count;
  return node;
}

ast_node_t* ast_create_function(char* name, parameter_t* params,
                                int param_count, type_t* return_type,
                                ast_node_t* body)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_FUNCTION;
  node->data_type = NULL;
  node->data.function.name = strdup(name);
  node->data.function.params = params;
  node->data.function.param_count = param_count;
  node->data.function.return_type = return_type;
  node->data.function.body = body;
  return node;
}

ast_node_t* ast_create_var_decl(char* name, type_t* type,
                                ast_node_t* initializer, int is_mutable)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_VAR_DECL;
  node->data_type = type;
  node->data.var_decl.name = strdup(name);
  node->data.var_decl.type = type;
  node->data.var_decl.initializer = initializer;
  node->data.var_decl.is_mutable = is_mutable;
  return node;
}

ast_node_t* ast_create_block(ast_node_t** statements, int count)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_BLOCK;
  node->data_type = NULL;
  node->data.block.statements = statements;
  node->data.block.statement_count = count;
  return node;
}

ast_node_t* ast_create_return(ast_node_t* expression)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_RETURN;
  node->data_type = NULL;
  node->data.return_stmt.expression = expression;
  return node;
}

ast_node_t* ast_create_if(ast_node_t* condition, ast_node_t* then_stmt,
                          ast_node_t* else_stmt)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_IF;
  node->data_type = NULL;
  node->data.if_stmt.condition = condition;
  node->data.if_stmt.then_stmt = then_stmt;
  node->data.if_stmt.else_stmt = else_stmt;
  return node;
}

ast_node_t* ast_create_while(ast_node_t* condition, ast_node_t* body)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_WHILE;
  node->data_type = NULL;
  node->data.while_stmt.condition = condition;
  node->data.while_stmt.body = body;
  return node;
}

ast_node_t* ast_create_binary_op(operator_t op, ast_node_t* left,
                                 ast_node_t* right)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_BINARY_OP;
  node->data_type = NULL;
  node->data.binary_op.op = op;
  node->data.binary_op.left = left;
  node->data.binary_op.right = right;
  return node;
}

ast_node_t* ast_create_unary_op(operator_t op, ast_node_t* operand)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_UNARY_OP;
  node->data_type = NULL;
  node->data.unary_op.op = op;
  node->data.unary_op.operand = operand;
  return node;
}

ast_node_t* ast_create_call(char* name, ast_node_t** args, int arg_count)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_CALL;
  node->data_type = NULL;
  node->data.call.name = strdup(name);
  node->data.call.args = args;
  node->data.call.arg_count = arg_count;
  return node;
}

ast_node_t* ast_create_identifier(char* name)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_IDENTIFIER;
  node->data_type = NULL;
  node->data.identifier.name = strdup(name);
  return node;
}

ast_node_t* ast_create_literal_int(int value)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_LITERAL_INT;
  node->data_type = type_create_int();
  node->data.literal_int.value = value;
  return node;
}

ast_node_t* ast_create_literal_bool(int value)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_LITERAL_BOOL;
  node->data_type = type_create_bool();
  node->data.literal_bool.value = value;
  return node;
}

ast_node_t* ast_create_literal_string(char* value)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_LITERAL_STRING;
  node->data_type = type_create_string();
  node->data.literal_string.value = strdup(value);
  return node;
}

ast_node_t* ast_create_assignment(ast_node_t* target, ast_node_t* value)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_ASSIGNMENT;
  node->data_type = NULL;
  node->data.assignment.target = target;
  node->data.assignment.value = value;
  return node;
}

ast_node_t* ast_create_struct_decl(char* name, field_t** fields,
                                   int field_count, method_t** methods,
                                   int method_count)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_STRUCT_DECL;
  node->data_type = NULL;
  node->data.struct_decl.name = strdup(name);
  node->data.struct_decl.fields = fields;
  node->data.struct_decl.field_count = field_count;
  node->data.struct_decl.methods = methods;
  node->data.struct_decl.method_count = method_count;
  return node;
}

ast_node_t* ast_create_field_access(ast_node_t* object, char* field_name)
{
  ast_node_t* node = malloc(sizeof(ast_node_t));
  node->type = AST_FIELD_ACCESS;
  node->data_type = NULL;
  node->data.field_access.object = object;
  node->data.field_access.field_name = strdup(field_name);
  return node;
}

void ast_free(ast_node_t* node)
{
  if (!node)
    return;

  switch (node->type)
  {
  case AST_PROGRAM:
    for (int i = 0; i < node->data.program.statement_count; i++)
    {
      ast_free(node->data.program.statements[i]);
    }

    free(node->data.program.statements);

    break;

  case AST_FUNCTION:
    free(node->data.function.name);

    for (int i = 0; i < node->data.function.param_count; i++)
    {
      parameter_free(&node->data.function.params[i]);
    }

    free(node->data.function.params);
    type_free(node->data.function.return_type);
    ast_free(node->data.function.body);
    break;

  case AST_VAR_DECL:
    free(node->data.var_decl.name);
    type_free(node->data.var_decl.type);
    ast_free(node->data.var_decl.initializer);
    break;

  case AST_BLOCK:
    for (int i = 0; i < node->data.block.statement_count; i++)
    {
      ast_free(node->data.block.statements[i]);
    }

    free(node->data.block.statements);

    break;

  case AST_RETURN:
    ast_free(node->data.return_stmt.expression);
    break;

  case AST_IF:
    ast_free(node->data.if_stmt.condition);
    ast_free(node->data.if_stmt.then_stmt);
    ast_free(node->data.if_stmt.else_stmt);
    break;

  case AST_WHILE:
    ast_free(node->data.while_stmt.condition);
    ast_free(node->data.while_stmt.body);
    break;

  case AST_BINARY_OP:
    ast_free(node->data.binary_op.left);
    ast_free(node->data.binary_op.right);
    break;

  case AST_UNARY_OP:
    ast_free(node->data.unary_op.operand);
    break;

  case AST_CALL:
    free(node->data.call.name);

    for (int i = 0; i < node->data.call.arg_count; i++)
    {
      ast_free(node->data.call.args[i]);
    }

    free(node->data.call.args);

    break;

  case AST_IDENTIFIER:
    free(node->data.identifier.name);
    break;

  case AST_LITERAL_STRING:
    free(node->data.literal_string.value);
    break;

  case AST_ASSIGNMENT:
    ast_free(node->data.assignment.target);
    ast_free(node->data.assignment.value);
    break;

  case AST_STRUCT_DECL:
    free(node->data.struct_decl.name);

    for (int i = 0; i < node->data.struct_decl.field_count; i++)
    {
      field_free(node->data.struct_decl.fields[i]);
    }

    free(node->data.struct_decl.fields);

    for (int i = 0; i < node->data.struct_decl.method_count; i++)
    {
      method_free(node->data.struct_decl.methods[i]);
    }

    free(node->data.struct_decl.methods);

    break;

  case AST_FIELD_ACCESS:
    ast_free(node->data.field_access.object);
    free(node->data.field_access.field_name);
    break;

  default:
    break;
  }

  type_free(node->data_type);
  free(node);
}

// @fix: there is some weird double free issue

void ast_print(ast_node_t* node, int indent)
{
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    printf("  ");

  switch (node->type)
  {
  case AST_PROGRAM:
    printf("Program\n");

    for (int i = 0; i < node->data.program.statement_count; i++)
    {
      ast_print(node->data.program.statements[i], indent + 1);
    }

    break;

  case AST_FUNCTION:
    printf("Function: %s\n", node->data.function.name);
    ast_print(node->data.function.body, indent + 1);
    break;

  case AST_VAR_DECL:
    printf("VarDecl: %s %s\n", node->data.var_decl.is_mutable ? "mut" : "let",
           node->data.var_decl.name);

    if (node->data.var_decl.initializer)
    {
      ast_print(node->data.var_decl.initializer, indent + 1);
    }

    break;

  case AST_BLOCK:
    printf("Block\n");

    for (int i = 0; i < node->data.block.statement_count; i++)
    {
      ast_print(node->data.block.statements[i], indent + 1);
    }

    break;

  case AST_RETURN:
    printf("Return\n");

    if (node->data.return_stmt.expression)
    {
      ast_print(node->data.return_stmt.expression, indent + 1);
    }

    break;

  case AST_IF:
    printf("If\n");
    ast_print(node->data.if_stmt.condition, indent + 1);
    ast_print(node->data.if_stmt.then_stmt, indent + 1);

    if (node->data.if_stmt.else_stmt)
    {
      ast_print(node->data.if_stmt.else_stmt, indent + 1);
    }

    break;

  case AST_WHILE:
    printf("While\n");
    ast_print(node->data.while_stmt.condition, indent + 1);
    ast_print(node->data.while_stmt.body, indent + 1);
    break;

  case AST_BINARY_OP:
    printf("BinaryOp: %d\n", node->data.binary_op.op);
    ast_print(node->data.binary_op.left, indent + 1);
    ast_print(node->data.binary_op.right, indent + 1);
    break;

  case AST_UNARY_OP:
    printf("UnaryOp: %d\n", node->data.unary_op.op);
    ast_print(node->data.unary_op.operand, indent + 1);
    break;

  case AST_CALL:
    printf("Call: %s\n", node->data.call.name);

    for (int i = 0; i < node->data.call.arg_count; i++)
    {
      ast_print(node->data.call.args[i], indent + 1);
    }

    break;

  case AST_IDENTIFIER:
    printf("Identifier: %s\n", node->data.identifier.name);
    break;

  case AST_LITERAL_INT:
    printf("LiteralInt: %d\n", node->data.literal_int.value);
    break;

  case AST_LITERAL_BOOL:
    printf("LiteralBool: %s\n",
           node->data.literal_bool.value ? "true" : "false");
    break;

  case AST_LITERAL_STRING:
    printf("LiteralString: %s\n", node->data.literal_string.value);
    break;

  case AST_ASSIGNMENT:
    printf("Assignment\n");
    ast_print(node->data.assignment.target, indent + 1);
    ast_print(node->data.assignment.value, indent + 1);
    break;

  case AST_STRUCT_DECL:
    printf("StructDecl: %s\n", node->data.struct_decl.name);

    for (int i = 0; i < node->data.struct_decl.field_count; i++)
    {
      for (int j = 0; j < indent + 1; j++)
        printf("  ");
      printf("Field: %s\n", node->data.struct_decl.fields[i]->name);
    }

    for (int i = 0; i < node->data.struct_decl.method_count; i++)
    {
      for (int j = 0; j < indent + 1; j++)
        printf("  ");
      printf("Method: %s\n", node->data.struct_decl.methods[i]->name);
    }

    break;

  case AST_FIELD_ACCESS:
    printf("FieldAccess: %s\n", node->data.field_access.field_name);
    ast_print(node->data.field_access.object, indent + 1);
    break;
  }
}
