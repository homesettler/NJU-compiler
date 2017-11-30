%{
    #include "Node.h"
    #include "lex.yy.c"
	#include "semantic.h"
    Node* root;
    char errorStr[256];
    //#define DEBUG 1
%}
%union {
    int address;
    Node *node;
}

%token <node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left  PLUS MINUS
%left STAR DIV
%left DOT LB RB LP RP
%right NOT

%nonassoc ELSE

%type <node> Program ExtDefList ExtDef ExtDecList Specifier
%type <node> StructSpecifier OptTag Tag VarDec FunDec VarList
%type <node> ParamDec CompSt StmtList Stmt DefList Def DecList
%type <node> Dec Exp Args

%%

/*=====================High-level Definitions=======================*/

Program : ExtDefList {$$ = initNode("Program","\0");addChild($$,$1);root = $$;}
    ;
ExtDefList : ExtDef ExtDefList {$$ = initNode("ExtDefList","\0");addChild($$,$2);addChild($$,$1);}
    |   /*empty*/ {$$ = NULL;}
    ;
ExtDef : Specifier ExtDecList SEMI { $$ = initNode("ExtDef","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    |   Specifier SEMI {$$ = initNode("ExtDef","\0");addChild($$,$2);addChild($$,$1);}
    |   Specifier FunDec CompSt {$$ = initNode("ExtDef","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
	|	Specifier FunDec SEMI {$$ = initNode("ExtDef","\0");addChild($$,$3);adChild($$,$2);addChild($$,$1);}
    |   error SEMI {errorCount++;printf("Error type B at Line %d: syntax error.\n",yylineno);}
    ;
ExtDecList : VarDec {$$ = initNode("ExtDecList","\0");addChild($$,$1);}
    |   VarDec  COMMA   ExtDecList { $$ = initNode("ExtDecList","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;

/*=====================Specifier====================*/
Specifier : TYPE {$$ = initNode("Specifier","\0");addChild($$,$1);}
    |   StructSpecifier {$$ = initNode("Specifier","\0");addChild($$,$1);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { $$  = initNode("StructSpecifier","\0");addChild($$,$5);addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    |   STRUCT Tag { $$ = initNode("StructSpecifier","\0");addChild($$,$2);addChild($$,$1);}
    ;
OptTag : ID {$$=initNode("OptTag","\0");addChild($$,$1);}
    |   /*empty*/ {$$=NULL;}
    ;
Tag : ID {$$=initNode("Tag","\0");addChild($$,$1);}
    ;

/*===================Declarators===================*/
VarDec : ID {$$=initNode("VarDec","\0");addChild($$,$1);}
    |   VarDec LB INT RB {$$=initNode("VarDec","\0");addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;
FunDec : ID LP VarList RP {$$=initNode("FunDec","\0");addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    |   ID LP RP {$$=initNode("FunDec","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;
VarList : ParamDec COMMA VarList {$$=initNode("VarList","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | ParamDec {$$=initNode("VarList","\0");addChild($$,$1);}
    ;
ParamDec : Specifier VarDec {$$=initNode("ParamDec","\0");addChild($$,$2);addChild($$,$1);} 
    ;


/*====================Statements==================*/
CompSt : LC DefList StmtList RC {$$=initNode("CompSt","\0");addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;
StmtList : Stmt StmtList {$$ = initNode("StmtList","\0");addChild($$,$2);addChild($$,$1);}
    | /*empty*/ {$$ = NULL;}
    ;
Stmt : Exp SEMI {$$=initNode("Stmt","\0");addChild($$,$2);addChild($$,$1);}
    | CompSt {$$=initNode("Stmt","\0");addChild($$,$1);}
    | RETURN Exp SEMI {$$=initNode("Stmt","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | IF LP Exp RP Stmt {$$=initNode("Stmt","\0");addChild($$,$5);addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | IF LP Exp RP Stmt ELSE Stmt {$$=initNode("Stmt","\0");addChild($$,$7);addChild($$,$6);addChild($$,$5);addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | WHILE LP Exp RP Stmt {$$=initNode("Stmt","\0");addChild($$,$5);addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp error SEMI {errorCount++;printf("Error type B at Line %d: Missing \";\".\n",yylineno);}
    | error SEMI {errorCount++;printf("Error type B at Line %d: syntax error.\n",yylineno);}
    ;

/*------------------Local Definitions-----------------*/
DefList : Def DefList {$$=initNode("DefList","\0");addChild($$,$2);addChild($$,$1);}
    | {$$=NULL;}
    ;
Def : Specifier DecList SEMI {$$=initNode("Def","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;
DecList : Dec {$$=initNode("DecList","\0");addChild($$,$1);}
    | Dec COMMA DecList {$$=initNode("DecList","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;
Dec : VarDec {$$=initNode("Dec","\0");addChild($$,$1);}
    | VarDec ASSIGNOP Exp {$$=initNode("VarDec","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    ;

/*----------------Expressions------------------*/
Exp : Exp ASSIGNOP Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp AND Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp OR Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp RELOP Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp PLUS Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp MINUS Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp STAR Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp DIV Exp {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | LP Exp RP {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | MINUS Exp {$$=initNode("Exp","\0");addChild($$,$2);addChild($$,$1);}
    | NOT Exp {$$=initNode("Exp","\0");addChild($$,$2);addChild($$,$1);}
    | ID LP Args RP {$$=initNode("Exp","\0");addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | ID LP RP {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp LB Exp RB {$$=initNode("Exp","\0");addChild($$,$4);addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp DOT ID {$$=initNode("Exp","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | ID {$$=initNode("Exp","\0");addChild($$,$1);}
    | INT {$$=initNode("Exp","\0");addChild($$,$1);}
    | FLOAT {$$=initNode("Exp","\0");addChild($$,$1);}
    | Exp LB error SEMI {errorCount++;printf("Error type B at Line %d: Missing \"]\".\n",yylineno);}
    ;
Args : Exp COMMA Args {$$=initNode("Args","\0");addChild($$,$3);addChild($$,$2);addChild($$,$1);}
    | Exp {$$=initNode("Args","\0");addChild($$,$1);}
    ;

%%

int main(int argc,char *argv[])
{
    if(argc<=1)
        return 1;
    FILE *fp = fopen(argv[1],"r");
    if(!fp)
    {
        printf("Open File failed\n");
        return 1;
    }
    root = NULL;
    yylineno = 1;
    yyrestart(fp);
#ifdef DEBUG
	yydebug = 1;
#endif
    yyparse();
    if(errorCount == 0)
    {
        //printTree(root,0);
		Program(root);
    }
    return 0;
}
int yyerror(char* msg){

}

