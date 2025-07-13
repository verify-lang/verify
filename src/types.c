#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

type_t* type_create_void(void)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_VOID;
  return type;
}

type_t* type_create_int(void)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_INT;
  return type;
}

type_t* type_create_bool(void)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_BOOL;
  return type;
}

type_t* type_create_string(void)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_STRING;
  return type;
}

type_t* type_create_function(type_t* return_type, type_t** param_types,
                             int param_count)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_FUNCTION;
  type->data.function.return_type = return_type;
  type->data.function.param_types = param_types;
  type->data.function.param_count = param_count;
  return type;
}

type_t* type_create_custom(char* name)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_CUSTOM;
  type->data.custom_name = strdup(name);
  return type;
}

void type_free(type_t* type)
{
  if (!type)
    return;

  if (type->kind == TYPE_CUSTOM)
  {
    free(type->data.custom_name);
  }

  if (type->kind == TYPE_FUNCTION)
  {
    type_free(type->data.function.return_type);

    for (int i = 0; i < type->data.function.param_count; i++)
    {
      type_free(type->data.function.param_types[i]);
    }

    free(type->data.function.param_types);
  }

  if (type->kind == TYPE_STRUCT)
  {
    free(type->data.struct_type.name);

    for (int i = 0; i < type->data.struct_type.field_count; i++)
    {
      field_free(type->data.struct_type.fields[i]);
    }

    free(type->data.struct_type.fields);

    for (int i = 0; i < type->data.struct_type.method_count; i++)
    {
      method_free(type->data.struct_type.methods[i]);
    }

    free(type->data.struct_type.methods);
  }

  free(type);
}

int type_equal(type_t* a, type_t* b)
{
  if (!a || !b)
    return 0;

  if (a->kind != b->kind)
    return 0;

  if (a->kind == TYPE_FUNCTION)
  {
    if (a->data.function.param_count != b->data.function.param_count)
      return 0;

    if (!type_equal(a->data.function.return_type, b->data.function.return_type))
      return 0;

    for (int i = 0; i < a->data.function.param_count; i++)
    {
      if (!type_equal(a->data.function.param_types[i],
                      b->data.function.param_types[i]))
      {
        return 0;
      }
    }
  }

  return 1;
}

char* type_to_string(type_t* type)
{
  if (!type)
    return strdup("unknown");

  switch (type->kind)
  {
  case TYPE_VOID:
    return strdup("void");
  case TYPE_INT:
    return strdup("Int");
  case TYPE_BOOL:
    return strdup("Bool");
  case TYPE_STRING:
    return strdup("String");
  case TYPE_CUSTOM:
    return strdup(type->data.custom_name);
  case TYPE_FUNCTION:
    return strdup("function");
  case TYPE_STRUCT:
    return strdup(type->data.struct_type.name);
  }

  return strdup("unknown");
}

type_t* type_create_struct(char* name, field_t** fields, int field_count,
                           method_t** methods, int method_count)
{
  type_t* type = malloc(sizeof(type_t));
  type->kind = TYPE_STRUCT;
  type->data.struct_type.name = strdup(name);
  type->data.struct_type.fields = fields;
  type->data.struct_type.field_count = field_count;
  type->data.struct_type.methods = methods;
  type->data.struct_type.method_count = method_count;
  return type;
}

field_t* field_create(char* name, type_t* type)
{
  field_t* field = malloc(sizeof(field_t));
  field->name = strdup(name);
  field->type = type;
  return field;
}

void field_free(field_t* field)
{
  if (!field)
    return;
  free(field->name);
  type_free(field->type);
  free(field);
}

method_t* method_create(char* name, parameter_t* params, int param_count,
                        type_t* return_type, struct ast_node* body)
{
  method_t* method = malloc(sizeof(method_t));
  method->name = strdup(name);
  method->params = params;
  method->param_count = param_count;
  method->return_type = return_type;
  method->body = body;
  return method;
}

void method_free(method_t* method)
{
  if (!method)
    return;
  free(method->name);
  for (int i = 0; i < method->param_count; i++)
  {
    parameter_free(&method->params[i]);
  }
  free(method->params);
  type_free(method->return_type);
  free(method);
}

parameter_t* parameter_create(char* name, type_t* type)
{
  parameter_t* param = malloc(sizeof(parameter_t));
  param->name = strdup(name);
  param->type = type;
  return param;
}

void parameter_free(parameter_t* param)
{
  if (!param)
    return;
  free(param->name);
  type_free(param->type);
  free(param);
}
