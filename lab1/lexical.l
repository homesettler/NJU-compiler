%{
    #include "Node.h"
    #include "syntax.tab.h"
    int errorCount = 0;
%}

comment_line "//"[^\n]*
comment_line_s "/*"(.|\r|\n)*?"*/"
comment_line_s_illegal "/*"?(.|\t|\n)*?"*/"(.|\r|\n)*?"*/"
ws [ \t]+
decimal ([1-9][0-9]*)|0
oct 0[0-7]*
oct_illegal 0[0-9]*
hex ("0x"|"0X")[0-9A-Fa-f]*
hex_illegal ("0x"|"0X")[0-9A-Za-z]*
integer {decimal}|{oct}|{hex}
float1 {decimal}"."[0-9]+
float2 [1-9]"."[0-9]*[Ee][+-]?[0-9]+
float_ill1 ({decimal}".")|("."{decimal})
float_ill2 [0-9]*"."[0-9]*[Ee][+-]?
id [a-zA-Z_][0-9a-zA-Z_]*
type int|float
relop ">"|"<"|">="|"<="|"=="|"!="

%option yylineno

%%

{comment_line} {;}
{comment_line_s} {;}
{comment_line_s_illegal} {errorCount++;printf("Error type B at line \"%d\": illegal comments.\n",yylineno);}
{ws} {;}
\n {;}
{oct_illegal} {errorCount++;printf("Error type B at line %d: illegal octal number \"%s\".\n",yylineno,yytext); return INT;}
{hex_illegal} {errorCount++;printf("Error type B at line %d: illegal hex number \"%s\".\n",yylineno,yytext);return INT;}
{integer} {yylval.node = initNode("INT",yytext);return INT;}
{float1} {yylval.node = initNode("FLOAT",yytext);return FLOAT;}
{float2} {yylval.node = initNode("FLOAT",yytext);return FLOAT;}
{float_ill1} {errorCount++;printf("Error type B at line %d: illegal float \"%s\".\n",yylineno,yytext);return FLOAT;}
{float_ill2} {errorCount++;printf("Error type B at line %d: illegal float \"%s\".\n",yylineno,yytext);return FLOAT;}
{type}  {yylval.node = initNode("TYPE",yytext);return TYPE;}
struct  {yylval.node = initNode("STRUCT",yytext);return STRUCT;}
return  {yylval.node = initNode("RETURN",yytext);return RETURN;}
if      {yylval.node = initNode("IF",yytext);return IF;}
else    {yylval.node = initNode("ELSE",yytext);return ELSE;}
while   {yylval.node = initNode("WHILE",yytext);return WHILE;}
{id}    {yylval.node = initNode("ID",yytext);return ID;}
{relop} {yylval.node = initNode("RELOP",yytext);return RELOP;}
"="     {yylval.node = initNode("ASSIGNOP",yytext);return ASSIGNOP;}
","     {yylval.node = initNode("COMMA",yytext);return COMMA;}
";"     {yylval.node = initNode("SEMI",yytext);return SEMI;}
"+"     {yylval.node = initNode("PLUS",yytext);return PLUS;}
"-"     {yylval.node = initNode("MINUS",yytext);return MINUS;}
"*"     {yylval.node = initNode("STAR",yytext);return STAR;}
"/"     {yylval.node = initNode("DIV",yytext);return DIV;}
"&&"    {yylval.node = initNode("AND",yytext);return AND;}
"||"    {yylval.node = initNode("OR",yytext);return OR;}
"."     {yylval.node = initNode("DOT",yytext);return DOT;}
"!"     {yylval.node = initNode("NOT",yytext);return NOT;}
"("     {yylval.node = initNode("LP",yytext);return LP;}
")"     {yylval.node = initNode("RP",yytext);return RP;}
"{"     {yylval.node = initNode("LC",yytext);return LC;}
"}"     {yylval.node = initNode("RC",yytext);return RC;}
"["     {yylval.node = initNode("LB",yytext);return LB;}
"]"     {yylval.node = initNode("RB",yytext);return RB;}
.       {errorCount++;printf("Error type A at Line %d: Mysterious character \"%s\".\n",yylineno,yytext);}

%%