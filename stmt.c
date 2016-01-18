#include "stmt.h"
#include "scope.h"
#include "decl.h"
#include "register.h"
#include "expr.h"
#include<stdio.h>
#include<stdlib.h>
int if_cont = -2;
int for_cont = -2;
struct stmt * stmt_create( stmt_kind_t kind, struct decl *d, struct expr *init_expr, struct expr *e, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next )
{
	struct stmt *t = malloc(sizeof(*t));
	t->kind = kind;
	t->decl = d;
	t->init_expr = init_expr;
	t->expr = e;
	t->next_expr = next_expr;
	t->body = body;
	t->else_body = else_body;
	t->next = next;
	return t;
}

void stmt_print( struct stmt *s, int indent )
{
	if(indent == 0)indent++;
	if(!s)return;
	switch(s->kind)
	{
 		case STMT_DECL:
			decl_print(s->decl,indent);
			break;
		case STMT_EXPR:
			identar(indent);
			expr_print(s->expr);
			printf(";\n");
			break;
		case STMT_IF_ELSE:
			identar(indent);
			printf("if(");	
			expr_print(s->expr);
			printf(")\n");	
			stmt_print(s->body,indent+1);
			if(s->else_body!=NULL)
			{
			   identar(indent);
			   printf("else\n");
			   stmt_print(s->else_body,indent+1);
			}
			break;
		case STMT_FOR:
			identar(indent);
			printf("for(");
			expr_print(s->init_expr);
			printf(";");
			expr_print(s->expr);
			printf(";");
			expr_print(s->next_expr);
			printf(")\n");
			stmt_print(s->body,indent+1);
			break;
		case STMT_PRINT:
			identar(indent);
			printf("print ");
			expr_print(s->expr);
			printf(";\n");
			break;
		case STMT_RETURN:
			identar(indent);
			printf("return ");
			expr_print(s->expr);
			printf(";\n");
			break;
		case STMT_BLOCK:
			identar(indent-1);
			printf("{\n");
			stmt_print(s->body,indent);
			identar(indent-1);
			printf("}\n");
		case STMT_SCOLON:
			break;
	}
	stmt_print(s->next,indent);
}

void identar(int ident)
{
     int i;
     for(i=0;i<ident;i++)printf("\t");	
}

void stmt_resolve(struct stmt *s,int x)
{
	if(!s)return;
	switch(s->kind)
	{
		case STMT_DECL :
			decl_resolve(s->decl,x);
			break;
		case STMT_IF_ELSE:
			expr_resolve(s->expr,x);
			stmt_resolve(s->body,x);
			stmt_resolve(s->else_body,x);
			break;
		case STMT_BLOCK:
			scope_enter();
			stmt_resolve(s->body,x);
			scope_leave();
			break;
		case STMT_EXPR:
		expr_resolve(s->expr,x);
			break;
		case STMT_PRINT:
			expr_resolve(s->expr,x);
			break;
		case STMT_FOR:
			expr_resolve(s->init_expr,x);
			expr_resolve(s->expr,x);
			expr_resolve(s->next_expr,x);
			stmt_resolve(s->body,x);
			break;
		case STMT_RETURN:
			expr_resolve(s->expr,x);
			break;
	        default:
			break;
	}
	stmt_resolve(s->next,x);
}

void stmt_typecheck(struct stmt *t,struct type *tipo){
	if(!t)return;
	struct type *retorno;
	switch(t->kind){
		case STMT_DECL:
			decl_typecheck(t->decl);
			break;
		case STMT_EXPR:
			expr_typecheck(t->expr);
			break;
		case STMT_IF_ELSE:
			//printf("if\n");
			expr_typecheck(t->expr);
			//printf("if\n");
			stmt_typecheck(t->body,tipo);
			//printf("if\n");
			stmt_typecheck(t->else_body,tipo);
			//printf("if\n");
			break;
		case STMT_FOR:
			expr_typecheck(t->init_expr);
			//printf("aa\n");
			expr_typecheck(t->expr);
			//printf("a\n");
			expr_typecheck(t->next_expr);	
			stmt_typecheck(t->body,tipo);
			//printf("a\n");
			break;
		case STMT_PRINT:
			expr_typecheck(t->expr);
			break;
		case STMT_RETURN:
			retorno = expr_typecheck(t->expr);
			if(retorno->kind != tipo->kind){
				get_incrementError(2);
				printf("Tried to return the ");
				type_print(retorno);
				printf(" ");
				expr_print(t->expr);
				printf(" in a function that was asking for a return of type ");
				type_print(tipo);
				printf("\n");	
			}
			break;
		case STMT_BLOCK:
			stmt_typecheck(t->body,tipo);
			break;
		default:

			break;
			
			
	}
	stmt_typecheck(t->next,tipo);
}

void stmt_codegen(struct stmt *s,FILE *file){
	if(!s)return;
	switch(s->kind){
		case STMT_DECL:
			decl_codegen(s->decl,file);
			break;
		case STMT_BLOCK:
			stmt_codegen(s->body,file);
			break;
		case STMT_EXPR:
			expr_codegen(s->expr,file);
			register_free(s->expr->reg);
			break;
		case STMT_RETURN:
			if(s->expr)expr_codegen(s->expr,file);
			if(s->expr)fprintf(file,"	movq %s,%%rax\n",register_name(s->expr->reg));
			if(s->expr)register_free(s->expr->reg);
			fprintf(file,"	jmp FUNCAO%d\n",get_total_function());
			break;
		case STMT_IF_ELSE:
			if_cont+=2;
			s->if_cont_aux = if_cont+1;
			expr_codegen(s->expr,file);	
			fprintf(file,"	cmp $0,%s\n	je L%d\n",register_name(s->expr->reg),if_cont);// greater than or equal
			register_free(s->expr->reg);
			stmt_codegen(s->body,file);						 	
			register_free(s->expr->reg);
			fprintf(file,"	jmp L%d\n",s->if_cont_aux);
			fprintf(file,"L%d:\n",s->if_cont_aux-1);
			stmt_codegen(s->else_body,file);
			register_free(s->expr->reg);
			fprintf(file,"L%d:\n",s->if_cont_aux);
			break;
		case STMT_FOR:
			for_cont+=2;
			s->for_cont_aux = for_cont+1;
			if(s->init_expr)expr_codegen(s->init_expr,file);
			if(s->init_expr)register_free(s->init_expr->reg);
			fprintf(file,"FOR%d:\n",for_cont);
			if(s->expr)expr_codegen(s->expr,file);
			if(s->expr)fprintf(file,"	cmp $0,%s\n",register_name(s->expr->reg));
			if(s->expr)fprintf(file,"	je FOR%d\n",s->for_cont_aux);
			if(s->expr)register_free(s->expr->reg);
			stmt_codegen(s->body,file);
			if(s->next_expr)expr_codegen(s->next_expr,file);
			fprintf(file,"	jmp FOR%d\nFOR%d:",s->for_cont_aux-1,s->for_cont_aux);
			break;
		case STMT_PRINT:
			stmt_printout(s->expr,file);
			break;
		default:
			break;
	}	
	stmt_codegen(s->next,file);
}

void stmt_printout(struct expr *e,FILE *file){
	if(!e)return;
	if(e->kind == EXPR_EXPR_LIST){
		stmt_printout(e->left,file);
		stmt_printout(e->right,file);
		return;
	}
	expr_codegen(e,file);
	fprintf(file,"	pushq %%r10\n");
	fprintf(file,"	pushq %%r11\n");
	fprintf(file,"	movq %s,%%rdi\n",register_name(e->reg));
	if(expr_typecheck(e)->kind  == TYPE_INTEGER)fprintf(file,"	call print_integer\n");
	else if(expr_typecheck(e)->kind  == TYPE_STRING){fprintf(file,"	call print_string\n");}
	else if(expr_typecheck(e)->kind  == TYPE_BOOLEAN){fprintf(file,"	call print_boolean\n");}
	else if(expr_typecheck(e)->kind  == TYPE_CHARACTER){fprintf(file,"	call print_character\n");}
	fprintf(file,"	popq %%r11\n");
	fprintf(file,"	popq %%r10\n");
	register_free(e->reg);	
}













