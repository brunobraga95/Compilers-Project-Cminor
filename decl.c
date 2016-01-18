#include "decl.h"
#include "decl.h"
#include "scope.h"
#include "hash_table.h"
#include "symbol.h"
#include "register.h"
#include<stdlib.h>
#include<stdio.h>
int param_count = 1;
int local_count = 1;
int const_expr = 0;
int resolve_error = 0;
int typecheck_error = 0;
struct hash_table *parametro = NULL;
int params_local_varibles = 0;
int local_variables_cont = 0;
int funcoes = 0;
void incremet_local_params_variables(){
	params_local_varibles++; 
}
	
int get_total_local_params_variables(){
	return params_local_varibles; 
}	
int get_total_function(){
	return funcoes; 
}	
	
struct decl * decl_create( char *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next )
{
      	struct decl *x = malloc(sizeof(*x));
	x->name = name;
	x->type = t;
	x->value = v;
	x->code = c;
	x->next = next;	
	return x;
}

void decl_print( struct decl *d, int indent )
{
     if(d==NULL)return;
	identar(indent);
    	printf("%s: ",d->name);
	type_print(d->type);
	
        if(d->code!=NULL)
	{
	  printf("={\n");
          stmt_print(d->code,indent+1);
	  identar(indent); 
          printf("}\n");		
        }
		else if(d->value!=NULL)
	{
	  printf("=");
	  expr_print(d->value);
	  printf(";\n");
	}
      else printf(";\n");
     decl_print(d->next,indent);	
}

void decl_resolve(struct decl* d,int x)
{
	if(!d)return;
	if(scope_lookup_local(d->name)!=NULL && d->type->kind!=TYPE_FUNCTION)
	{
		if(x == 1)printf("Resolve error: %s already declared\n",d->name);
		get_incrementError(1);
	}
	else{
		if(scope_level() == 1){
			struct symbol *sym = symbol_create(SYMBOL_GLOBAL,d->type,d->name,0,0);
			d->symbol = sym;
			scope_bind(d->name,sym);
		}
		else {
			params_local_varibles++;
			local_variables_cont++;	
			struct symbol *sym = symbol_create(SYMBOL_LOCAL,d->type,d->name,0,0);
			d->symbol = sym;
			scope_bind(d->name,sym);
		}
		expr_resolve(d->value,x);
		if(d->type->kind == TYPE_FUNCTION){
		  	param_list_resolve(d->type->params,0);
			if(!parametro)parametro = hash_table_create(0,0);
			hash_table_insert(parametro,d->name,d->type);
		}
		if(d->code){
			scope_enter();
			stmt_resolve(d->code,x);
			d->symbol->total_locals = local_variables_cont;
			local_variables_cont = 0;
			params_local_varibles = 0;
			scope_leave();	
		}
		if(d->next){
			decl_resolve(d->next,x);
		}
	}	
}
void decl_typecheck(struct decl *d){
	if(!d)return;
	if(d->value){
		if(d->type->kind!=TYPE_ARRAY){
			struct type* t;
			t = expr_typecheck(d->value);
			if(d->type-> kind != t->kind){
				printf("Cannot assign ");
				expr_print(d->value);
				printf(" to ");
				type_print(d->type);
				printf("\n");
				get_incrementError(2);
			}
		}else
		{
			if(d->value->kind!=EXPR_ARRAY_LIST){
				printf("Cannot assign ");
				expr_print(d->value);
				printf(" to array %s\n",d->name);
				get_incrementError(2);
			}
			else{
				struct type* aux_type = d->type;
				int lenght = 1;
				if(aux_type->expression){
					while(aux_type->kind == TYPE_ARRAY){
				 		 if(aux_type->expression && aux_type->expression->kind!= EXPR_INTEGER_LITERAL){
						  printf("Array %s must have a literal value as index\n",d->name);
						  lenght = 99999;
						  break;	
						}
						else{
						  lenght*=aux_type->expression->literal_value;
						}
						aux_type = aux_type->subtype;
			     	
					}
				}
				else{
					lenght = 99999;
					while(aux_type->kind == TYPE_ARRAY)aux_type = aux_type->subtype;
			   	}


				resolve_array_values(d->value,aux_type,d->name);	
				if(const_expr > lenght){
					get_incrementError(2);
					printf("Too many arguments on inicialization of array %s\n",d->name);
				}
			}
		}
	}
	
	if(d->symbol->kind == SYMBOL_GLOBAL)
	{
		const_expr = 0;
		expr_constant(d->value);
		if(const_expr>0){
		  get_incrementError(1);
		  printf("Tried to assign not constant variable in the declaration of %s\n",d->name);
		}
				
	}
		
	if(d->code)stmt_typecheck(d->code,d->type->subtype);
	decl_typecheck(d->next);

}
struct hash_table* getParams(){return parametro;}

void expr_constant(struct expr *e)
{
	if(!e || const_expr > 0)return;
	expr_constant(e->left);
	expr_constant(e->right);
	if(e->kind == EXPR_NAME)const_expr++;
}

int get_incrementError(int kind){
	if(kind == 1){
		resolve_error++;
		return resolve_error;
	}else if(kind == 2){
		typecheck_error++;
		return typecheck_error;
	}else if(kind == 3)return resolve_error;
	else return typecheck_error;
}

void resolve_array_values(struct expr *e,struct type *t,const char *name)
{
	if(!e)return;
	if(e->kind == EXPR_EXPR_LIST || e->kind == EXPR_ARRAY_LIST ){resolve_array_values(e->left,t,name); resolve_array_values(e->right,t,name);}
	else{
		const_expr++;	
		struct type *aux2 = expr_typecheck(e);
		if(aux2->kind != t->kind){
			get_incrementError(2);
			printf("Passing wrong argument to array %s: Passing int the array expression list ",name);
			type_print(aux2);
			printf(" ");
			expr_print(e);
			printf("\n");
		}
	
	}
}


void decl_codegen(struct decl *d, FILE * file){
	if(!d)return;
	if(d->symbol->kind == SYMBOL_GLOBAL){
		if(!d->code && !d->value && d->type->kind != TYPE_FUNCTION){
			if(d->type->kind != TYPE_STRING)fprintf(file,".data\n%s: .quad 0\n",d->name);
			else{
				
				//printf("teste");
				fprintf(file,".data\n.global %s\n%s:\n	.string \"\" \n",d->name,d->name);
			}
		}
		else{
			if(d->value && d->type->kind != TYPE_FUNCTION){
				if(d->type->kind == TYPE_STRING && d->type->kind != TYPE_FUNCTION){
					fprintf(file,".data\n.global %s\n%s:\n	.string ""%s""\n",d->name,d->name,d->value->string_literal);//nao esquecer de tratar quebras de linha	  	
				}
				else if(d->type->kind != TYPE_FUNCTION) fprintf(file,".data\n.global %s\n%s:\n	.quad %d\n",d->name,d->name,d->value->literal_value);//pode ter expressoes do tipo 10+1 na declaracao?
			}else{
				if(d->code){
				fprintf(file,".text\n.global %s\n%s:\n",d->name,d->name);
				struct param_list *p = d->type->params;
				fprintf(file,"	pushq %%rbp\n	movq  %%rsp, %%rbp\n");
				int parameters_cont = 0;
				while(p){
					p = p->next;
					parameters_cont++;
				}
				if(parameters_cont >= 1){
				   	fprintf(file,"	pushq %%rdi\n");	
				}
				if(parameters_cont >= 2){
				  	fprintf(file,"	pushq %%rsi\n");
				}
				if(parameters_cont >= 3){
					fprintf(file,"	pushq %%rdx\n");
				}
				if(parameters_cont >= 4){
					fprintf(file,"	pushq %%rcx\n");
				}
				if(parameters_cont >= 5){
					fprintf(file,"	pushq %%r8\n");
				}
				if(parameters_cont == 6){
					fprintf(file,"	pushq %%r9\n");
				}
				if(d->symbol->total_locals > 0){		
					fprintf(file,"	subq $%d,%%rsp\n",d->symbol->total_locals*8);
				}
				fprintf(file,"	pushq %%rbx\n");
				fprintf(file,"	pushq %%r12\n");
				fprintf(file,"	pushq %%r13\n");
				fprintf(file,"	pushq %%r14\n");
				fprintf(file,"	pushq %%r15\n\n");		
				stmt_codegen(d->code,file);
				fprintf(file,"FUNCAO%d:\n",funcoes);
				fprintf(file,"\n");
				fprintf(file,"	popq %%r15\n");
				fprintf(file,"	popq %%r14\n");
				fprintf(file,"	popq %%r13\n");
				fprintf(file,"	popq %%r12\n");
				fprintf(file,"	popq %%rbx\n");
				fprintf(file,"	movq %%rbp,%%rsp\n");
				fprintf(file,"	popq %%rbp\n");
				fprintf(file,"	ret\n");
				funcoes++;	
				//postamble como tratar
				}
			}
		}

    }
	else if(d->symbol->kind == SYMBOL_LOCAL){
			if(d->value){
				expr_codegen(d->value,file);
				fprintf(file,"	movq %s,-%d(%%rbp)\n",register_name(d->value->reg),d->symbol->param_local_variables*8);//arrumar isso, voce usou valor aleatorio,
				register_free(d->value->reg);
				//dont forget to free registers
				//voltar no video do dia 20, minuto 35 para tratar o caso das strings
			}
		}
	decl_codegen(d->next,file);
}















































