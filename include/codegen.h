#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Transforms/Utils.h>
#include <llvm-c/BitWriter.h>
#include "ast.h"

typedef struct symbol_table_entry
{
  char* name;
  LLVMValueRef value;
  type_t* type;
  int is_global;
  struct symbol_table_entry* next;
} symbol_table_entry_t;

typedef struct symbol_table
{
  symbol_table_entry_t* entries;
  struct symbol_table* parent;
} symbol_table_t;

typedef struct struct_info
{
  char* name;
  LLVMTypeRef llvm_type;
  field_t** fields;
  int field_count;
  method_t** methods;
  int method_count;
  struct struct_info* next;
} struct_info_t;

typedef struct import_info
{
  char* module_path;
  char* alias;
  LLVMModuleRef module;
  symbol_table_t* symbols;
  struct import_info* next;
} import_info_t;

typedef struct codegen_context
{
  LLVMContextRef context;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
  LLVMTargetMachineRef target_machine;
  symbol_table_t* current_scope;
  struct_info_t* struct_types;
  import_info_t* imports;
  LLVMValueRef current_function;
  int string_counter;
  char* module_name;
} codegen_context_t;

codegen_context_t* codegen_context_create(const char* module_name);
void codegen_context_free(codegen_context_t* ctx);

#endif
