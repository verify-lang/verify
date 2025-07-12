#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

extern ast_node_t* root;
extern int yyparse(void);
extern int yylex(void);
extern void yyerror(const char* msg);

#endif
