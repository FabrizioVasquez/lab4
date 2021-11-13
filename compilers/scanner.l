%{
#include "Enums.h"
int yylinenod = 1;
const char *ss;
void yyerror(const char *str)
{
    fprintf(stdout,"The token %s, is not specified for the program.\nYou can find it on the line %d\n",str,yylinenod);
}

void yyerrorM(const char *str)
{
    fprintf(stdout,"The token %s, you should remove it .\nYou can find it on the line %d\n",str,yylinenod);
}

%}

%option noyywrap c++

DIGIT [1-9]
DIGIT_ [0-9]
OP_SUMA ["+"]
OP_RESTA [-]
OP_MULTIPLICACION ["*"]
P_ABIERTO ["("]
P_CERRADO [")"]
LETRA [a-z]

%%
[+-]?{DIGIT}{DIGIT_}* return Categoria::NUMBER;
{OP_SUMA} return Categoria::OP_SUMA;
{OP_RESTA} return Categoria::OP_RESTA;
{OP_MULTIPLICACION} return Categoria::OP_MULTIPLICACION;
{P_ABIERTO} return Categoria::P_ABIERTO;
{P_CERRADO} return Categoria::P_CERRADO;
"a" return Categoria::A;
"b" return Categoria::B;
\n  {++yylinenod;}
["ç"|"@"] { yyerror(yytext); return Categoria::ERROR;}
{LETRA}+ { yyerrorM(yytext); return Categoria::ERROR;}
[\t\r]  ;
.   ;
%%
