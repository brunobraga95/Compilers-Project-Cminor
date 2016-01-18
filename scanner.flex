%{
#include "token.h"
#include "parser.tab.h"
#include<stdio.h>
#include<stdlib.h>
%}
delim			[ \t\n\r]
ws				{delim}+
digito		    [0-9]
letter			[A-Za-z]
ctint			{digito}+
id				({letter}|_)({letter}|{digito}|_)*
ctreal		    {digito}+\.{digito}*([Ee][+-]?{digito}+)?
carac1		    \\.|[^\\']
carac2		    \\.|[^\\\"\n]
ctcarac		    '{carac1}'
cadeia		    \"{carac2}*\"
%%
"//"[^\n]*\n {;}
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] {;}
string {return TOKEN_STRING;}
{ws} {;}
'letters' {return TOKEN_INTEGER_LITERAL;}
integer {return TOKEN_INTEGER;}
char {return TOKEN_CHAR;}
do {return TOKEN_DO;}
false {return TOKEN_FALSE;}
while {return TOKEN_WHILE;}
if {return TOKEN_IF;}
else {return TOKEN_ELSE;}
array {return TOKEN_ARRAY;}
boolean {return TOKEN_BOOLEAN;}
for {return TOKEN_FOR;}
function {return TOKEN_FUNCTION;}
print {return TOKEN_PRINT;}
return {return TOKEN_RETURN;}
true {return TOKEN_TRUE;}
void {return TOKEN_VOID;}
[0-9]* {return TOKEN_INTEGER_LITERAL;}
{id}  {
	if(strlen(yytext) > 256)
	   {
		fprintf(stderr,"Scan error: Identifier %s too large\n",yytext);
		exit(EXIT_FAILURE);
	   }	

return TOKEN_IDENTIFIER;}
{ctcarac} {return TOKEN_CHARACTER_LITERAL;}
{cadeia}  {
	
	int cont = 0,i;
	    for(i = 1;i<strlen(yytext)-1;i++)
	    {
		if(yytext[i] == '\\')i++;
		cont++;
	    }
	    //printf("%d",cont);
	    if(cont>=256){
		fprintf(stderr,"Scan error: %s string too large\n",yytext);		
		exit(EXIT_FAILURE );
	}
return TOKEN_STRING_LITERAL;
}
";" {return TOKEN_SCOLON;}
">=" {return TOKEN_GE;}
"<=" {return TOKEN_LE;}
">" {return TOKEN_GT;}
"<" {return TOKEN_LT;}
"||" {return TOKEN_OR;}
"&&" {return TOKEN_AND;}
"!" {return TOKEN_NOT;}
"==" {return TOKEN_EQ;}
"!=" {return TOKEN_NE;}
"++"	{return TOKEN_INCREMENT;}
"--"	{return TOKEN_DECREMENT;}
"+" {return TOKEN_ADD;}
"-" {return TOKEN_SUB;}
"*" {return TOKEN_MULT;}
"/" {return TOKEN_DIV;}
"%" {return TOKEN_MOD;}
"~" {return TOKEN_NEG;}
"("	{return TOKEN_OPPAR;}
")"	{return TOKEN_CLPAR;}
"["	{return TOKEN_OPBRAK;}
"]"	{return TOKEN_CLBRAK;}
"{"	{return TOKEN_OPBRACE;}
"}"	{return TOKEN_CLBRACE;}
"="	{return TOKEN_ASSIGN;}
"^"	{return TOKEN_EXPONENTIATION;}
":"	{return TOKEN_COLON;}
"," 	{return TOKEN_COMMA;}
. {fprintf(stderr,"Scan error: %s is not a valid character\n",yytext);
    exit(EXIT_FAILURE );;	
return TOKEN_ERROR;}
%%
int yywrap(){return 1;}
