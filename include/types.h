#ifndef TYPES_H
#define TYPES_H

typedef enum
{
  TYPE_VOID,
  TYPE_INT,
  TYPE_BOOL,
  TYPE_STRING,
  TYPE_FUNCTION
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
  } data;
} type_t;

typedef struct
{
  char* name;
  type_t* type;
} parameter_t;

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

#endif
