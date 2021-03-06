
/*
Declare token types at the top of the bison file,
causing them to be automatically generated in parser.tab.h
for use by scanner.c.
*/

%token TOKEN_UNARY_NEGATION
%token TOKEN_STRING
%token TOKEN_INTEGER
%token TOKEN_CHAR
%token TOKEN_DO
%token TOKEN_FALSE
%token TOKEN_WHILE
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_ARRAY
%token TOKEN_BOOLEAN
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING_LITERAL
%token TOKEN_CHARACTER_LITERAL
%token TOKEN_INTEGER_LITERAL
%token TOKEN_TRUE
%token TOKEN_VOID
%token TOKEN_IDENTIFIER
%token TOKEN_SCOLON
%token TOKEN_GE
%token TOKEN_LE
%token TOKEN_GT
%token TOKEN_LT
%token TOKEN_OR
%token TOKEN_AND
%token TOKEN_NOT
%token TOKEN_EQ
%token TOKEN_NE
%token TOKEN_INCREMENT
%token TOKEN_DECREMENT
%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_MULT
%token TOKEN_DIV
%token TOKEN_NEG
%token TOKEN_ASSIGN
%token TOKEN_EXPONENTIATION
%token TOKEN_MOD
%token TOKEN_COLON
%token TOKEN_COMMA
%token TOKEN_OPPAR
%token TOKEN_CLPAR
%token TOKEN_OPBRAK
%token TOKEN_CLBRAK
%token TOKEN_OPBRACE
%token TOKEN_CLBRACE


%union {
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct param_list *param_list;
	struct type *type;
	char *c;
};
%type <decl> program decl_list decl
%type <stmt> stmt stmt_list match_stmt not_empty_stmt_list stmt_list2
%type <expr> expr_list paren and_expr or_expr unary_expr expo_expr add_expr array_elements mult_expr expr primary_expr op_expr expr_list2 expr_list expr_list_array compare_expr inc_expr
%type <param_list> param_list not_empty_param_list param_list2
%type <type> type type2
%type <c> ident 

%{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "expr.h"
#include "type.h"
#include "param_list.h"
#include "decl.h"
#include "stmt.h"

/*
YYSTYPE is the lexical value returned by each rule in a bison grammar.
By default, it is an integer. In this example, we are returning a pointer to an expression.
*/
/*
Clunky: Manually declare the interface to the scanner generated by flex. 
*/

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );

/*
Clunky: Keep the final result of the parse in a global variable,
so that it can be retrieved by main().
*/

struct decl *parser_result = 0;


%}

%%

/* Here is the grammar: program is the start symbol. */

program : decl_list
		{parser_result = $1; return 0; }
	;

decl_list : decl decl_list{

	      $$ = $1;
	      $1->next = $2;	
	    }
	  | 
	    {$$ = 0;} 
	  ;

decl 	: ident TOKEN_COLON type TOKEN_ASSIGN expr TOKEN_SCOLON
	  {$$ = decl_create($1,$3,$5,0,0);}
	| ident TOKEN_COLON TOKEN_ARRAY TOKEN_OPBRAK TOKEN_CLBRAK type2 TOKEN_ASSIGN expr TOKEN_SCOLON
	  {$$ = decl_create($1,type_create(TYPE_ARRAY,0,$6,0),$8,0,0);}
	| ident TOKEN_COLON type TOKEN_SCOLON/*prototype*/
	  {$$ = decl_create($1,$3,0,0,0);}
	| ident TOKEN_COLON type TOKEN_ASSIGN TOKEN_OPBRACE stmt_list TOKEN_CLBRACE
	  {$$ = decl_create($1,$3,0,$6,0);}
        ;

type 	: TOKEN_STRING
	  {$$ = type_create(TYPE_STRING,0,0,0);}
	| TOKEN_INTEGER
	  {$$ = type_create(TYPE_INTEGER,0,0,0);}
	| TOKEN_CHAR
	  {$$ = type_create(TYPE_CHARACTER,0,0,0);}
	| TOKEN_BOOLEAN
	  {$$ = type_create(TYPE_BOOLEAN,0,0,0);} 
        | TOKEN_ARRAY TOKEN_OPBRAK expr TOKEN_CLBRAK type
	  {$$ = type_create(TYPE_ARRAY,0,$5,$3);}

	| TOKEN_FUNCTION type2 TOKEN_OPPAR param_list TOKEN_CLPAR
	  {$$ = type_create(TYPE_FUNCTION,$4,$2,0);}
	| TOKEN_VOID
	  {$$ = type_create(TYPE_VOID,0,0,0);}
	;


type2 	: TOKEN_STRING
	  {$$ = type_create(TYPE_STRING,0,0,0);}
	| TOKEN_INTEGER
	  {$$ = type_create(TYPE_INTEGER,0,0,0);}
	| TOKEN_CHAR
	  {$$ = type_create(TYPE_CHARACTER,0,0,0);}
	| TOKEN_BOOLEAN
	  {$$ = type_create(TYPE_BOOLEAN,0,0,0);} 
        | TOKEN_ARRAY TOKEN_OPBRAK op_expr TOKEN_CLBRAK type2
	  {$$ = type_create(TYPE_ARRAY,0,$5,$3);}
	| TOKEN_FUNCTION type TOKEN_OPPAR param_list TOKEN_CLPAR
	  {$$ = type_create(TYPE_FUNCTION,$4,$2,0);}
	| TOKEN_VOID
	  {$$ = type_create(TYPE_VOID,0,0,0);}
	;


param_list : /* nothing */
	     {$$ = 0;}	
	   | not_empty_param_list
  	     {$$ = $1;}  
	   ;

not_empty_param_list: ident TOKEN_COLON type2;
		      {$$ = param_list_create($1,$3,0);}
                     | param_list2
		      {$$ = $1;}
                    ; 

param_list2 : ident TOKEN_COLON type2 TOKEN_COMMA not_empty_param_list
	      {$$ = param_list_create($1,$3,$5);}
            ;

expr_list : //nothing
	    {$$ = 0;}
	  | expr_list2
            {$$ = $1;} 
          ;   


expr_list_array : expr_list2
		  {$$ = $1;}
           ;   

    	
expr_list2 : expr TOKEN_COMMA expr_list2 
	     {$$ = expr_create(EXPR_EXPR_LIST,$1,$3);}	
	   | expr
           {$$ = $1;}
           ;

ident : TOKEN_IDENTIFIER
	{
	char *c;
	c = strdup(yytext);
	$$ = c;}
      ;

stmt :decl
       {$$ = stmt_create(STMT_DECL,$1,0,0,0,0,0,0);}	
     | expr TOKEN_SCOLON
       {$$ = stmt_create(STMT_EXPR,0,0,$1,0,0,0,0);}
     | TOKEN_FOR TOKEN_OPPAR op_expr TOKEN_SCOLON op_expr TOKEN_SCOLON op_expr TOKEN_CLPAR stmt
       {$$ = stmt_create(STMT_FOR,0,$3,$5,$7,$9,0,0);}
     | TOKEN_OPBRACE stmt_list TOKEN_CLBRACE
       {$$ = stmt_create(STMT_BLOCK,0,0,0,0,$2,0,0);}
     | TOKEN_IF TOKEN_OPPAR expr TOKEN_CLPAR match_stmt TOKEN_ELSE stmt
       {$$ = stmt_create(STMT_IF_ELSE,0,0,$3,0,$5,$7,0);}
     | TOKEN_IF TOKEN_OPPAR expr TOKEN_CLPAR stmt
	{$$ = stmt_create(STMT_IF_ELSE,0,0,$3,0,$5,0,0);}
     | TOKEN_RETURN op_expr TOKEN_SCOLON
	{$$ = stmt_create(STMT_RETURN,0,0,$2,0,0,0,0);}
     | TOKEN_PRINT expr_list TOKEN_SCOLON
	{$$ = stmt_create(STMT_PRINT,0,0,$2,0,0,0,0);}
     ;

match_stmt: TOKEN_IF TOKEN_OPPAR expr TOKEN_CLPAR match_stmt TOKEN_ELSE match_stmt
	    {$$ = stmt_create(STMT_IF_ELSE,0,0,$3,0,$5,$7,0);}
          | TOKEN_RETURN op_expr TOKEN_SCOLON
            {$$ = stmt_create(STMT_RETURN,0,0,$2,0,0,0,0);}
     	  | TOKEN_PRINT expr_list TOKEN_SCOLON
            {$$ = stmt_create(STMT_PRINT,0,0,$2,0,0,0,0);}
	  | TOKEN_OPBRACE stmt_list TOKEN_CLBRACE 
            {$$ = stmt_create(STMT_BLOCK,0,0,0,0,$2,0,0);}
	  | expr TOKEN_SCOLON
            {$$ = stmt_create(STMT_EXPR,0,0,$1,0,0,0,0);}
	  | decl
            {$$ = stmt_create(STMT_DECL,$1,0,0,0,0,0,0);}
	  | TOKEN_FOR TOKEN_OPPAR op_expr TOKEN_SCOLON op_expr TOKEN_SCOLON op_expr TOKEN_CLPAR match_stmt
            {$$ = stmt_create(STMT_FOR,0,$3,$5,$7,$9,0,0);}

stmt_list : /* nothing */
	    {$$ = 0;}
	   | not_empty_stmt_list  
	    {$$ = $1;}
           ;

not_empty_stmt_list: stmt;
		     {$$ = $1;}
                    | stmt_list2
		     {$$ = $1;}
                    ; 

stmt_list2 : stmt not_empty_stmt_list
	     {$$ = $1;
	      $1->next = $2; }
	   ;        

op_expr ://nothing
	  {$$ = 0;}
	| expr
	  {$$ = $1;}
	;
	

//arrumar primary_expn

expr	: or_expr
	  {$$ = $1;}
	 | expr TOKEN_ASSIGN or_expr
	  {$$ = expr_create(EXPR_ASSIGN,$1,$3);}

         ;

or_expr : or_expr TOKEN_OR and_expr
	  {$$ = expr_create(EXPR_OR,$1,$3);}
	 | and_expr
	   {$$ = $1;}	
	 ;

and_expr : and_expr TOKEN_AND compare_expr
	   {$$ = expr_create(EXPR_AND,$1,$3);}
	 | compare_expr
	   {$$ = $1;}
	 ;


compare_expr : compare_expr TOKEN_GE add_expr
	       {$$ = expr_create(EXPR_GE,$1,$3);}
	     | compare_expr TOKEN_GT add_expr
	       {$$ = expr_create(EXPR_GT,$1,$3);}
	     | compare_expr TOKEN_LE add_expr
	       {$$ = expr_create(EXPR_LE,$1,$3);}
	     | compare_expr TOKEN_LT add_expr
	       {$$ = expr_create(EXPR_LT,$1,$3);}
	     | compare_expr TOKEN_NE add_expr
	       {$$ = expr_create(EXPR_NE,$1,$3);}
	     | compare_expr TOKEN_EQ add_expr
	       {$$ = expr_create(EXPR_EQ,$1,$3);}	
	     | add_expr
	       {$$ = $1;}
	     ;

add_expr : add_expr TOKEN_ADD mult_expr
	   {$$ = expr_create(EXPR_ADD,$1,$3);}
	 | add_expr TOKEN_SUB mult_expr
	   {$$ = expr_create(EXPR_SUB,$1,$3);}
	 | mult_expr
	   {$$ = $1;}
	 ;

mult_expr : mult_expr TOKEN_MULT expo_expr
	    {$$ = expr_create(EXPR_MULT,$1,$3);}
	   | mult_expr TOKEN_DIV expo_expr
	    {$$ = expr_create(EXPR_DIV,$1,$3);}
           | mult_expr TOKEN_MOD expo_expr
	    {$$ = expr_create(EXPR_MOD,$1,$3);}
	   | expo_expr
	    {$$ = $1;}
  	  ;

expo_expr : expo_expr TOKEN_EXPONENTIATION unary_expr 
	    {$$ = expr_create(EXPR_EXPONENTIATION,$1,$3);}
	  | unary_expr
	    {$$ = $1;}
	  ;

unary_expr: TOKEN_NOT inc_expr
	    {$$ = expr_create(EXPR_UNARY_NEGATION,0,$2);}
	  | TOKEN_SUB inc_expr
	    {$$ = expr_create(EXPR_MINUS,0,$2);}
          | inc_expr	
	    {$$ = $1;}
	  ;

inc_expr  : paren TOKEN_INCREMENT 
	    {$$ = expr_create(EXPR_INCREMENT,$1,0);}
	  | paren TOKEN_DECREMENT
	    {$$ = expr_create(EXPR_DECREMENT,$1,0);}
	   | paren
	    {$$ = $1;}
paren	: primary_expr
	   {$$ = $1;}	
	 | TOKEN_OPPAR expr TOKEN_CLPAR
	    {$$ = expr_create(EXPR_OPPAR,0,$2);}
	  ;
primary_expr : ident
               {$$ = expr_create_name($1);}
	     | ident TOKEN_OPBRAK expr TOKEN_CLBRAK array_elements
		{$$ = expr_create(EXPR_ARRAY_IDENT,expr_create_name($1),expr_create(EXPR_ARRAY_ELEMENT,$3,$5));}
             | ident TOKEN_OPPAR expr_list TOKEN_CLPAR
	       {$$ = expr_create(EXPR_FUNC,expr_create_name($1),$3);}	
	     | TOKEN_INTEGER_LITERAL
               {$$ = expr_create_integer_literal(atoi(strdup(yytext)));}
	     | TOKEN_STRING_LITERAL
               {$$ = expr_create_string_literal(strdup(yytext));}                                                  
	     | TOKEN_CHARACTER_LITERAL
               {$$ = expr_create_character_literal(strdup(yytext));}                                                       
	     | TOKEN_TRUE
               {$$ = expr_create_boolean_literal(1);}
	     | TOKEN_FALSE
                {$$ = expr_create_boolean_literal(0);}
	     | TOKEN_OPBRACE expr_list_array TOKEN_CLBRACE 
               {$$ = expr_create(EXPR_ARRAY_LIST,0,$2);}
	     ;

array_elements:TOKEN_OPBRAK expr TOKEN_CLBRAK array_elements
	   {$$ = expr_create(EXPR_ARRAY_ELEMENT,$2,$4);}
	  |{$$ = 0;}	
	  ;


%%


//This function will be called by bison if the parse should
//encounter an error.  In principle, "str" will contain something
//useful.  In practice, it often does not.


int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	exit(EXIT_FAILURE );
}
