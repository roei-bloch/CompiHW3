%{
    #include "output.hpp"
    #include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap
digit ([0-9])
letter ([a-zA-Z])
whitespace ([\t\n\r ])
hex (x([0-9A-Fa-f]{2}))
%x STR

%%

int                       {return INT;}
byte                      {return BYTE;}
b                         {return B;}
bool                      {return BOOL;}
and                       {return AND;}
or                        {return OR;}
not                       {return NOT;}
true                      {return TRUE;}
false                     {return FALSE;}
return                    {return RETURN;}
if                        {return IF;}
else                      {return ELSE;}
while                     {return WHILE;}
break                     {return BREAK;}
continue                  {return CONTINUE;}
;                         {return SC;}
\(                        {return LPAREN;}
\)                        {return RPAREN;} 
\{                          {return LBRACE;}
\}                         {return RBRACE;}
=                         {return ASSIGN;} 
\<|\>|\<=|\>=             {return BLE;}
==|\!=                    {return EQU;}
\+                        {return PLUS_MINUS;}
\-                        {return PLUS_MINUS;}
\*                        {return MULT_DIVIDE;}
\/                        {return MULT_DIVIDE;}



[a-zA-Z][a-zA-Z0-9]*      {return ID;}
([1-9][0-9]*)|0           {return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\" {return STRING;}
[ \r\n\t]+                {}
\/\/[^\r\n]*[\r|\n|\r\n]?   {}
.                         {output::errorLex(yylineno); exit(0);}

%%

