#include "type.h"
#include "stmt.h"
#include "decl.h"
#include "expr.h"
#include<stdio.h>
#include<stdlib.h>

struct type * type_create( type_kind_t kind, struct param_list *params, struct type *subtype, struct expr *e)
{
	struct type *t = malloc(sizeof(*t));
	t->kind = kind;
	t->params = params;
	t->subtype = subtype;
	t->expression = e;
	return t;
}
void type_print( struct type *t )
{
	if(!t)return;
	switch(t->kind)
	{
	   case TYPE_VOID:
		printf("void");
		break;
	   case TYPE_INTEGER:
		printf("integer");
		break;
	   case TYPE_BOOLEAN:
		printf("boolean");
		break;
	   case TYPE_STRING:
		printf("string");
		break;	
	   case TYPE_CHARACTER:
		printf("char");
		break;
	   case TYPE_ARRAY:
		printf("array [");
		expr_print(t->expression);
		printf("] ");
		type_print(t->subtype);
		break;
	   case TYPE_FUNCTION:
		printf("function ");
		type_print(t->subtype);
		printf(" (");
		param_list_print(t->params);
	        printf(" )");	
		break;
	default:
		break;
	
	  
	}
}
/*
struct type * expr_typecheck(struct expr *e){
	if(!e)type_create(TYPE_VOID,0,0,0);
	switch(e->kind){
		case EXPR_INTEGER_LITERAL:
			return type_create(TYPE_INTEGER,0,0,0);
			//colocar todos os tipos de literais
		case EXPR_NAME:
			return e->symbol->type;
		case EXPR_FUNCTION:
			return e->left->symbol->type->subtype;
		case EXPR_ADD:
			struct type *L = expr_typecheck(e->left);
			struct type *L = expr_typecheck(e->left);
			if(L->type!=TYPE_INTEGER || R->type!=TYPE_INTEGER)
			{
				printf("cannot add ");
				type_print(L);
				printf("to a");
				type_print(R);
			}
	}

}
*/	