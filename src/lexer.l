%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.h"
#include "y.tab.h"
%}

%option noyywrap
%option yylineno

%%

"def"           { return DEF; }
"let"           { return LET; }
"const"           { return CONST; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }
"true"          { yylval.bool_val = 1; return BOOL_LITERAL; }
"false"         { yylval.bool_val = 0; return BOOL_LITERAL; }
"Int"           { return INT_TYPE; }
"Bool"          { return BOOL_TYPE; }
"String"        { return STRING_TYPE; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.string_val = strdup(yytext); return IDENTIFIER; }

[0-9]+          { yylval.int_val = atoi(yytext); return INT_LITERAL; }

\"[^\"]*\"      { 
                  yylval.string_val = strndup(yytext + 1, strlen(yytext) - 2); 
                  return STRING_LITERAL; 
                }

"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }
"->"            { return ARROW; }

"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"%"             { return '%'; }
"="             { return '='; }
"<"             { return '<'; }
">"             { return '>'; }
"!"             { return '!'; }
"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }
";"             { return ';'; }
","             { return ','; }
":"             { return ':'; }

[ \t\n]+        { /* ignore whitespace */ }

.               { fprintf(stderr, "unknown character: %c\n", yytext[0]); }

%%
