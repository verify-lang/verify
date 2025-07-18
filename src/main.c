#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
#include "codegen.h"

extern FILE* yyin;
extern ast_node_t* root;

int main(int argc, char* argv[])
{
  codegen_context_t* ctx = codegen_context_create("test");
  codegen_context_free(ctx);
  return 0;
  if (argc > 1)
  {
    yyin = fopen(argv[1], "r");

    if (!yyin)
    {
      fprintf(stderr, "error: cannot open file %s\n", argv[1]);
      return 1;
    }
  }
  else
  {
    yyin = stdin;
  }

  if (yyparse() == 0)
  {
    if (root)
    {
      printf("AST:\n");
      ast_print(root, 0);
      ast_free(root);
    }
  }
  else
  {
    fprintf(stderr, "error: cannot parse source %s\n", argv[1]);
    return 1;
  }

  if (yyin != stdin)
  {
    fclose(yyin);
  }

  return 0;
}
