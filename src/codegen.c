#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

codegen_context_t* codegen_context_create(const char* module_name)
{
  codegen_context_t* ctx = malloc(sizeof(codegen_context_t));
  ctx->context = LLVMContextCreate();
  ctx->module = LLVMModuleCreateWithNameInContext(module_name, ctx->context);
  ctx->builder = LLVMCreateBuilderInContext(ctx->context);
  ctx->target_machine = NULL;
  ctx->current_scope = NULL;
  ctx->struct_types = NULL;
  ctx->imports = NULL;
  ctx->current_function = NULL;
  ctx->string_counter = 0;
  ctx->module_name = strdup(module_name);

  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();

  return ctx;
}

void codegen_context_free(codegen_context_t* ctx)
{
  if (!ctx)
    return;

  if (ctx->target_machine)
  {
    LLVMDisposeTargetMachine(ctx->target_machine);
  }

  LLVMDisposeBuilder(ctx->builder);
  LLVMDisposeModule(ctx->module);
  LLVMContextDispose(ctx->context);
  free(ctx->module_name);
  free(ctx);
}
