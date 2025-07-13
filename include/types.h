#ifndef TYPES_H
#define TYPES_H

typedef enum
{
  TYPE_VOID,
  TYPE_INT,
  TYPE_BOOL,
  TYPE_STRING,
  TYPE_FUNCTION,
  TYPE_STRUCT
} type_kind_t;

typedef struct type
{
  type_kind_t kind;
  union
  {
    struct
    {
      struct type* return_type;
      struct type** param_types;
      int param_count;
    } function;

    struct
    {
      char* name;
      struct field** fields;
      int field_count;
      struct method** methods;
      int method_count;
    } struct_type;
  } data;

} type_t;

typedef struct
{
  char* name;
  type_t* type;
} parameter_t;

typedef struct field
{
  char* name;
  type_t* type;
} field_t;

typedef struct method
{
  char* name;
  parameter_t* params;
  int param_count;
  type_t* return_type;
  struct ast_node* body;
} method_t;

type_t* type_create_void(void);
type_t* type_create_int(void);
type_t* type_create_bool(void);
type_t* type_create_string(void);
type_t* type_create_function(type_t* return_type, type_t** param_types,
                             int param_count);

void type_free(type_t* type);
int type_equal(type_t* a, type_t* b);
char* type_to_string(type_t* type);

parameter_t* parameter_create(char* name, type_t* type);
void parameter_free(parameter_t* param);

type_t* type_create_struct(char* name, field_t** fields, int field_count,
                           method_t** methods, int method_count);
field_t* field_create(char* name, type_t* type);
void field_free(field_t* field);
method_t* method_create(char* name, parameter_t* params, int param_count,
                        type_t* return_type, struct ast_node* body);
void method_free(method_t* method);

#endif
