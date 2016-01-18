#include "expr.h"
#include "scope.h"
#include "symbol.h"
#include "hash_table.h"
#include "register.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int flag = 0;
int cont_aux = 0;
int param_list = 0;
int arguments[10] = {0,0,0,0,0,0,0,0,0,0},cont[10] = {0,0,0,0,0,0,0,0,0,0};
int topo = -1,arguments_cont = 0;
struct hash_table *variables = NULL;
int reloap_cont = 0;
int strings = 0;
int arguments2 = 0;
struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right )
{
     	struct expr *t = malloc(sizeof(*t));
	t->kind = kind;
	t->left=left;
	t->right=right;
	return t;
}

struct expr* expr_create_boolean_literal(int c){
    struct expr *t = malloc(sizeof(*t));
	t->kind = EXPR_BOOLEAN_LITERAL;
	t->left=NULL;
	t->right=NULL;
	t->literal_value = c;
	return t;

}


struct expr* expr_create_name(const char *n){
    struct expr *t = malloc(sizeof(*t));
	t->kind = EXPR_NAME;
	t->left=NULL;
	t->right=NULL;
	t->name = strdup(n);
	return t;

}


struct expr* expr_create_integer_literal(int c){
    struct expr *t = malloc(sizeof(*t));
	t->kind = EXPR_INTEGER_LITERAL;
	t->left=NULL;
	t->right=NULL;
	t->literal_value = c;
	return t;
}


struct expr* expr_create_character_literal(const char *str ){
    struct expr *t = malloc(sizeof(*t));
	t->kind = EXPR_CHAR_LITERAL;
	t->left=NULL;
	t->right=NULL;
	t->string_literal = strdup(str);
	//strcpy(t->string_literal,str);
	return t;

}


struct expr* expr_create_string_literal(const char *str ){
    struct expr *t = malloc(sizeof(*t));
	t->kind = EXPR_STRING_LITERAL;
	t->left=NULL;
	t->right=NULL;
	t->string_literal = strdup(str);
	//strcpy(t->string_literal,str);
	return t;

}
void expr_print( struct expr *e )
{
   	if(e==NULL)return;
	//printf("(");
	expr_print(e->left);
	int isArrayList = 0;
	switch(e->kind) {
		case EXPR_ADD:
			printf("+");
			break;
		case EXPR_ASSIGN:
			printf("=");
			break;
		case EXPR_SUB:
			printf("-");
			break;
		case EXPR_MINUS:
			printf("-");
			break;
		case EXPR_MULT:
			printf("*");
			break;
		case EXPR_DIV:
			printf("/");
			break;
		case EXPR_MOD:
			printf("%%");
			break;
		case EXPR_NEG:
			printf("!");
			break;
		case EXPR_GT:
			printf(">");
			break;
		case EXPR_GE:
			printf(">=");
			break;
		case EXPR_LT:
			printf("<");
			break;
		case EXPR_LE:
			printf("<=");
			break;
		case EXPR_EQ:
			printf(" == ");
			break;
		case EXPR_NE:
			printf("!=");
			break;
		case EXPR_INCREMENT:
			printf("++");
			break;
		case EXPR_DECREMENT:
			printf("--");
			break;
		case EXPR_OR:
			printf(" || ");
			break;
		case EXPR_AND:
			printf(" && ");
			break;
		case EXPR_EXPONENTIATION:
			printf("^");
			break;
		case EXPR_BOOLEAN_LITERAL:
			if(e->literal_value == 1)printf("true");
			else printf("false");
			break;
		case EXPR_NAME:
			printf("%s",e->name);
			break;
		case EXPR_INTEGER_LITERAL:
			printf("%d",e->literal_value);
			break;
		case EXPR_CHAR_LITERAL:
			printf("%s",e->string_literal);
			break;
		case EXPR_STRING_LITERAL:
			printf("%s",e->string_literal);
			break;
		case EXPR_EXPR_LIST:
			printf(",");
			break;
		case EXPR_FUNC:
			printf("(");
			expr_print(e->right);
			printf(")");
			return;
			break;
		case EXPR_UNARY_NEGATION:
			printf("!");
			break;
		case EXPR_ARRAY_LIST:
			isArrayList=1;
			break;
		case EXPR_OPPAR:
			printf("(");
			expr_print(e->right);
			printf(")");
			return;
			break;
		case EXPR_ARRAY_ELEMENT:
			printf("]");
			if(e->right!=NULL)printf("[");
			break;
		default:
			break;
	}
	if(isArrayList == 1){
	   printf("{");
	   expr_print(e->right);
	   printf("}");
	}
	
	else{
	    if(e->kind == EXPR_ARRAY_IDENT){
	        printf("[");
		expr_print(e->right); 
	    }
	    else if(EXPR_ARRAY_ELEMENT){
		expr_print(e->right);
	    }
	    else expr_print(e->right);  		 	
	}
	
	//printf(")");  	
}

void expr_resolve(struct expr *e,int x)
{
	if(!e)return;
	expr_resolve(e->left,x);
	expr_resolve(e->right,x);
	if(e->kind == EXPR_NAME){
		struct symbol *sy = malloc(sizeof(*sy));
		sy = scope_lookup(e->name);
		if(sy){
			e->symbol = sy;
			if(sy->kind == SYMBOL_GLOBAL && x == 1)printf("%s resolves for global %s\n",e->name,e->name);
			if(sy->kind == SYMBOL_LOCAL){
			   if(x==1)printf("%s resolves for local %d\n",e->name,e->symbol->which);	
			}
			if(sy->kind == SYMBOL_PARAM){
			   if(x==1)printf("%s resolves for param %d\n",e->name,e->symbol->which);	
			}
			
		}
		else{
			get_incrementError(1);
			if(x==1)printf("Resolve error: %s is undefined\n", e->name);
			//error_count++;
		}
	}


}

struct type *expr_typecheck(struct expr *e){
	if(!e)return type_create(TYPE_VOID,0,0,0);
	struct type *L = expr_typecheck(e->left);
	struct type *R = expr_typecheck(e->right);
	struct symbol *s = malloc(sizeof(*s));
	struct hash_table * h;
	struct type *tipo;  
	struct param_list *p;  
	switch(e->kind){
		case EXPR_INTEGER_LITERAL:
			return type_create(TYPE_INTEGER,0,0,0);
			break;
		case EXPR_STRING_LITERAL:
			return type_create(TYPE_STRING,0,0,0);
			break;
		case EXPR_CHAR_LITERAL:
			return type_create(TYPE_CHARACTER,0,0,0);
			break;
		case EXPR_BOOLEAN_LITERAL:
			return type_create(TYPE_BOOLEAN,0,0,0);
			break;
		case EXPR_NAME:
			return type_create(e->symbol->type->kind,0,0,0);
			break;
		case EXPR_FUNC:
			topo++;
			arguments_cont = 0;
			cont_aux = 0;
			h = getParams();
			tipo = hash_table_lookup(h,e->left->name);
			p = tipo->params;	
			while(p){p = p->next;arguments_cont++;}
			arguments[topo] = arguments_cont;
			tipo = hash_table_lookup(h,e->left->name);
			p = tipo->params;
			test_params_function(e->right,p,e->left->name);
			if(arguments[topo] < cont[topo]){
				get_incrementError(2);
				printf("Passing too many arguments in function %s\n",e->left->name);
				return type_create(tipo->subtype->kind,0,0,0);
			}
			else if(arguments[topo] > cont[topo]){
				get_incrementError(2);
				printf("Missing arguments in function %s\n",e->left->name	);
				return type_create(tipo->subtype->kind,0,0,0);
			}
			arguments[topo] = 0;
			cont[topo] = 0;
			topo--;
			return type_create(tipo->subtype->kind,0,0,0);
		break;
		case EXPR_ADD:
			if(L->kind!= TYPE_INTEGER || R->kind!= TYPE_INTEGER )
			{
				expr_print(e);
				printf("Cannot add the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf(" to the ");
				type_print(R);
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER ,0,0,0);
			break;
		case EXPR_SUB:
			if(L->kind!= TYPE_INTEGER || R->kind!= TYPE_INTEGER )
			{
				printf("Cannot subtract the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf(" to the ");
				type_print(R);
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER ,0,0,0);
			break;
		case EXPR_MULT:
			if(L->kind!= TYPE_INTEGER || R->kind!= TYPE_INTEGER )
			{
				printf("Cannot multiply the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf(" to the ");
				type_print(R);
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER ,0,0,0);
			break;
		case EXPR_DIV:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER )
			{
				printf("Cannot divide the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf(" to the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf("\n");
				//error_count++;
			}
			return type_create(TYPE_INTEGER ,0,0,0);
			break;
		case EXPR_EXPONENTIATION:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER )
			{
				printf("Cannot elevate the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf(" to the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER ,0,0,0);
			break;
		case EXPR_MOD:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER )
			{
				printf("Cannot evaluate the rest of the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf(" by the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER ,0,0,0);
			break;

		case EXPR_MINUS:
			if(expr_typecheck(e->right)->kind != TYPE_INTEGER)
			{
				printf("Cannot multiply the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" by minus 1\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER,0,0,0);
			break;
		case EXPR_UNARY_NEGATION:
			if(expr_typecheck(e->right)->kind != TYPE_BOOLEAN)
			{
				printf("Cannot negate the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_BOOLEAN	,0,0,0);
			break;
		case EXPR_INCREMENT:
			if(L->kind != TYPE_INTEGER)
			{
				printf("Cannot increment the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER,0,0,0);
			break;
		case EXPR_DECREMENT:
			if(L->kind != TYPE_INTEGER)
			{
				printf("Cannot decrement the ");
				type_print(L);
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_INTEGER,0,0,0);
			break;
		case EXPR_ASSIGN:
			if(L->kind!=R->kind){
				printf("Cannot make the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf(" equals to the ");
				type_print(R);
				printf(" ");
				expr_print(e->right);
				printf("\n");
				get_incrementError(2);
			}	
			return type_create(expr_typecheck(e->left)->kind,0,0,0);
			break;
		case EXPR_EQ:
			if(L->kind!=R->kind){
				printf("Cannot compare the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf("\n");
				get_incrementError(2);

			}	
			return type_create(TYPE_BOOLEAN,0,0,0);
			break;
		case EXPR_NE:
			if(L->kind!=R->kind){
				get_incrementError(2);
				printf("Cannot compare the ");
				type_print(R);
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf("\n");

			}	
			return type_create(TYPE_BOOLEAN,0,0,0);
			break;
		case EXPR_LT:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER ){
				get_incrementError(2);
				printf("Cannot compare the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf("\n");

			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;
		case EXPR_LE:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER ){
				get_incrementError(2);
				printf("Cannot compare the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf("\n");

			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;
		case EXPR_GT:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER ){
				get_incrementError(2);
				printf("Cannot compare the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf("\n");

			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;
		case EXPR_GE:
			if(expr_typecheck(e->left)->kind!= TYPE_INTEGER  || expr_typecheck(e->right)->kind!= TYPE_INTEGER ){
				get_incrementError(2);
				printf("Cannot compare the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf("\n");

			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;
		case EXPR_AND:
			if(L->kind!= TYPE_BOOLEAN  || R->kind!= TYPE_BOOLEAN ){
				printf("Cannot compare the ");
				type_print(R);
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(L);
				printf(" ");
				expr_print(e->left);
				printf("\n");
				get_incrementError(2);
			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;
		case EXPR_OR:
			if(expr_typecheck(e->left)->kind!= TYPE_BOOLEAN  || expr_typecheck(e->right)->kind!= TYPE_BOOLEAN ){
				get_incrementError(2);
				printf("Cannot compare the ");
				type_print(expr_typecheck(e->right));
				printf(" ");
				expr_print(e->right);
				printf(" with the ");
				type_print(expr_typecheck(e->left));
				printf(" ");
				expr_print(e->left);
				printf("\n");
				get_incrementError(2);

			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;

		case EXPR_NEG:
			if(expr_typecheck(e->right)->kind!= TYPE_BOOLEAN){
				printf("Cannot negate the ");
				//type_print(expr_typecheck(e->right));
				//printf(" ");
				//expr_print(e->right);
				printf("\n");

			}
			return type_create(TYPE_BOOLEAN,0,0,0);	
			break;
		case EXPR_EXPR_LIST:
			expr_typecheck(e->left);
			expr_typecheck(e->right);
			return type_create(TYPE_VOID,0,0,0);	
			break;
		case EXPR_OPPAR:
			R = expr_typecheck(e->right);
			return type_create(R->kind,0,0,0);	
			break;
		case EXPR_ARRAY_IDENT:
			s = scope_lookup(e->left->name);
			tipo = s->type;
			if(tipo->kind == TYPE_STRING){
				struct type *t = expr_typecheck(e->right->left);
				if(t->kind !=TYPE_INTEGER)
				{
					printf("Index for the string %s needs to be and integer\n",e->left->name);
					get_incrementError(2);
				}
			 	return type_create(TYPE_CHARACTER,0,0,0);
			}
			while(tipo->kind == TYPE_ARRAY){
				tipo = tipo->subtype;
			}
			return type_create(tipo->kind,0,0,0);	
			break;
		default:
			break;	
	}
	return type_create(TYPE_VOID,0,0,0);
} 

void test_params_function(struct expr *e,struct param_list *p,const char *name){
	
	if(!e || !p)return;
	if(e->kind == EXPR_EXPR_LIST ){test_params_function(e->left,p,name);test_params_function(e->right,p,name);}
	else{
	
		cont[topo]++;
		struct param_list *aux = p;
		int i;
		for(i=0;i<cont[topo]-1;i++){
		   aux = aux->next;
		   if(!aux){
			return;
		   }		
		}
		struct type *aux2 = expr_typecheck(e);
		if(aux2->kind != aux->type->kind){
			get_incrementError(2);
			printf("Passing wrong argument to function %s: Passing the ",name);
			type_print(aux2);
			printf(" ");
			expr_print(e);
			printf(" expected a ");
			type_print(aux->type);
			printf("\n");
		}
	}
}
void expr_codegen(struct expr *e, FILE *file){
	struct type *tipo;
	struct hash_table * h;
	struct call *c;
	if(!e)return;
	expr_print(e);
	printf("\n");
	switch(e->kind){
		case EXPR_ASSIGN:
			//expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			if(e->left->kind == EXPR_NAME)fprintf(file,"	movq %s,%s\n",register_name(e->right->reg),symbol_code(e->left->symbol));
			else{
				fprintf(file,"	movq %s,%s\n",register_name(e->right->reg),symbol_code(e->left->right->symbol));
				//e->lef->right->reg = e->right->reg;
				expr_codegen(e->left,file);
			}
			e->reg = e->right->reg;
			break;
		case EXPR_INTEGER_LITERAL:
			e->reg = register_alloc();
			fprintf(file,"	movq $%d,%s\n",e->literal_value,register_name(e->reg));
			break;
		case EXPR_BOOLEAN_LITERAL:
			e->reg = register_alloc();
			fprintf(file,"	movq $%d,%s\n",e->literal_value,register_name(e->reg));
			break;
		case EXPR_CHAR_LITERAL:
			e->reg = register_alloc();
			fprintf(file,"	movq $%s,%s\n",e->string_literal,register_name(e->reg));
		break;
		case EXPR_NAME:
			e->reg = register_alloc();
			if(e->symbol->type->kind!=TYPE_STRING || (e->symbol->type->kind==TYPE_STRING && e->symbol->kind != SYMBOL_GLOBAL))fprintf(file,"	movq %s,%s\n",symbol_code(e->symbol),register_name(e->reg));
			else fprintf(file,"	leaq %s,%s\n",symbol_code(e->symbol),register_name(e->reg));
			break;
		case EXPR_STRING_LITERAL:
			e->reg = register_alloc();//ta dando ruim na string
			fprintf(file,".data\n.str%d:	.string %s\n.text\n	leaq .str%d, %s\n",strings,e->string_literal,strings,register_name(e->reg));
			strings++;
			break;
		case EXPR_ADD:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	addq %s,%s\n",register_name(e->left->reg),register_name(e->right->reg));		
			e->reg = e->right->reg;//perguntar pro neil, nao deveria ser left?
			register_free(e->left->reg);
			break;
		case EXPR_SUB:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	subq %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));		
			e->reg = e->left->reg;//perguntar pro neil, nao deveria ser left?
			register_free(e->right->reg);
			break;
		case EXPR_MINUS:
			expr_codegen(e->right,file);
			fprintf(file,"	movq $-1,%%rax\n");
			fprintf(file,"	imulq %s\n",register_name(e->right->reg));		
			fprintf(file,"	movq %%rax,%s\n",register_name(e->right->reg));
			e->reg = e->right->reg;
			break;
		case EXPR_MULT:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	movq %s,%%rax\n",register_name(e->left->reg));
			fprintf(file,"	imulq %s\n",register_name(e->right->reg));		
			fprintf(file,"	movq %%rax,%s\n",register_name(e->right->reg));
			register_free(e->left->reg);
			e->reg = e->right->reg;
			break;
		case EXPR_DIV:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	movq %s,%%rax\n",register_name(e->left->reg));
			fprintf(file,"	cdq\n");
			fprintf(file,"	idivq %s\n",register_name(e->right->reg));
			fprintf(file,"	movq %%rax,%s\n",register_name(e->right->reg));
			register_free(e->left->reg);
			e->reg = e->right->reg;
			break;
		case EXPR_MOD:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	movq %s,%%rax\n",register_name(e->left->reg));
			fprintf(file,"	cdq\n");
			fprintf(file,"	idivq %s\n",register_name(e->right->reg));
			fprintf(file,"	movq %%rdx,%s\n",register_name(e->right->reg));
			register_free(e->left->reg);
			e->reg = e->right->reg;
			break;
		default:
		case EXPR_INCREMENT:
			expr_codegen(e->left,file);
			fprintf(file,"	movq %s,%s\n",register_name(e->left->reg),register_name(e->reg));
			fprintf(file,"	addq $1,%s\n",register_name(e->left->reg));		
			fprintf(file,"	movq %s,%s\n",register_name(e->left->reg),symbol_code(e->left->symbol));
			break;
		case EXPR_DECREMENT:
			expr_codegen(e->left,file);
			fprintf(file,"	movq %s,%s\n",register_name(e->left->reg),register_name(e->reg));
			fprintf(file,"	subq $1,%s\n",register_name(e->left->reg));		
			fprintf(file,"	movq %s,%s\n",register_name(e->left->reg),symbol_code(e->left->symbol));
			break;
		case EXPR_LT:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));
			fprintf(file,"	jl R%d\n",reloap_cont);
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont+1);
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont+1);
			e->reg = e->right->reg;
			register_free(e->left->reg); 	
			reloap_cont+=2;
			break;
		case EXPR_GT: 						//nao se esqueca de testar todos os reloaps
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));
			fprintf(file,"	jg R%d\n",reloap_cont); // diferenca entre jmp e jl
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont+1);
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont+1);
			e->reg = e->right->reg;
			register_free(e->left->reg); 
			reloap_cont+=2;
			break;
		case EXPR_LE:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));
			fprintf(file,"	jle R%d\n",reloap_cont);
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont+1);
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont+1);
			e->reg = e->right->reg;
			register_free(e->left->reg); 
			reloap_cont+=2;
			break;
		case EXPR_GE:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));
			fprintf(file,"	jge R%d\n",reloap_cont);
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont+1);
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont+1);
			e->reg = e->right->reg;
			register_free(e->left->reg); 
			reloap_cont+=2;
			break;
		case EXPR_EQ:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));
			fprintf(file,"	je R%d\n",reloap_cont);
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont+1);
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont+1);
			register_free(e->left->reg);
			e->reg = e->right->reg;
			reloap_cont+=2;
			break;
		case EXPR_NE:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp %s,%s\n",register_name(e->right->reg),register_name(e->left->reg));
			fprintf(file,"	je R%d\n",reloap_cont);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont+1);
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont+1);
			e->reg = e->right->reg;
			register_free(e->left->reg); 
			reloap_cont+=2;
			break;
		case EXPR_AND:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp $0,%s\n",register_name(e->left->reg));
			fprintf(file,"	je R%d\n",reloap_cont+1);
			fprintf(file,"	cmp $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	je R%d\n",reloap_cont+1);	
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont);
			fprintf(file,"R%d:\n",reloap_cont+1);
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont);
			e->reg = e->right->reg;
			register_free(e->left->reg); 
			reloap_cont+=2;
			break;
		case EXPR_OR:
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			fprintf(file,"	cmp $1,%s\n",register_name(e->left->reg));
			fprintf(file,"	je R%d\n",reloap_cont+1);
			fprintf(file,"	cmp $1,%s\n",register_name(e->right->reg));
			fprintf(file,"	je R%d\n",reloap_cont+1);	
			fprintf(file,"	movq $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont);
			fprintf(file,"R%d:\n",reloap_cont+1);
			fprintf(file,"	movq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"R%d:\n",reloap_cont);
			reloap_cont+=2;
			break;
		case EXPR_EXPONENTIATION:
			/*
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			e->reg = register_alloc();
			fprintf(file,"	movq $1,%s\n",register_name(e->reg));
			fprintf(file,"R%d:\n",reloap_cont);
			fprintf(file,"	cmp $0,%s\n",register_name(e->right->reg));
			fprintf(file,"	je R%d\n",reloap_cont+1);
			fprintf(file,"	subq $1,%s\n",register_name(e->right->reg));
			fprintf(file,"	movq %s,%%rax\n",register_name(e->reg));
			fprintf(file,"	imulq %s\n",register_name(e->left->reg));		
			fprintf(file,"	movq %%rax,%s\n",register_name(e->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont);
			fprintf(file,"R%d:\n",reloap_cont+1);
			register_free(e->left->reg);
			register_free(e->right->reg);
			reloap_cont+=2;
			*/
			expr_codegen(e->left,file);
			expr_codegen(e->right,file);
			e->reg = register_alloc();
			fprintf(file,"	pushq %%r10\n");
			fprintf(file,"	pushq %%r11\n");
			fprintf(file,"	movq %s,%%rdi\n",register_name(e->left->reg));
			fprintf(file,"	movq %s,%%rsi\n",register_name(e->right->reg));
			fprintf(file,"	call integer_power\n");
			fprintf(file,"	popq %%r11\n");
			fprintf(file,"	popq %%r10\n");
			fprintf(file,"	movq %%rax,%s\n",register_name(e->reg));
			register_free(e->left->reg);
			register_free(e->right->reg);	
			break;
		case EXPR_UNARY_NEGATION:
			e->reg = register_alloc();
			expr_codegen(e->right,file);
			fprintf(file,"	cmp $1,%s\n",register_name(e->right->reg));
			fprintf(file,"	je R%d\n",reloap_cont+1);
			fprintf(file,"	movq $1,%s\n",register_name(e->reg));
			fprintf(file,"	jmp R%d\n",reloap_cont);
			fprintf(file,"	R%d:\n",reloap_cont+1);
			fprintf(file,"	movq $0,%s\n",register_name(e->reg));
			fprintf(file,"R%d:\n",reloap_cont);
			register_free(e->right->reg);
			reloap_cont+=1;
			break;
		case EXPR_OPPAR:
			expr_codegen(e->right,file);
			e->reg = e->right->reg;
			break;
				
		case EXPR_FUNC:            
			arguments2 = 0;
			e->reg = register_alloc();
			c = NULL;
			arguments_solve(e->right,file,&c);
			fprintf(file,"	pushq %%r10\n");
			fprintf(file,"	pushq %%r11\n");
			arguments2 = 0;
			while(c){
				if(arguments2 == 0){
					fprintf(file,"	movq %s,%%rdi\n",register_name(c->reg));
					register_free(c->reg);
				}
				if(arguments2 == 1){
					fprintf(file,"	movq %s,%%rsi\n",register_name(c->reg));
					register_free(c->reg);
				}
				if(arguments2 == 2){
					fprintf(file,"	movq %s,%%rdx\n",register_name(c->reg));
					register_free(c->reg);
				}
				if(arguments2 == 3){
					fprintf(file,"	movq %s,%%rcx\n",register_name(c->reg));
					register_free(c->reg);	
				}
				if(arguments2 == 4){
					fprintf(file,"	movq %s,%%r8\n",register_name(c->reg));
					register_free(c->reg);	
				}
				if(arguments2 == 5){
					fprintf(file,"	movq %s,%%r9\n",register_name(c->reg));	
					register_free(c->reg);
				}
				if(arguments2>5){
					printf("Too many arguments\n");
					exit(0);
				}
				arguments2++;
				c = c->next;
			}
			fprintf(file,"	call %s\n",e->left->name);
			fprintf(file,"	popq %%r11\n");
			fprintf(file,"	popq %%r10\n");
			fprintf(file,"	movq %%rax,%s\n",register_name(e->reg));
			h = getParams();
			arguments2 = 0;
			tipo = hash_table_lookup(h,e->left->name);
			if(tipo->subtype->kind == TYPE_VOID)register_free(e->reg);	
			break;
}



}
void arguments_solve(struct expr *e,FILE *file,struct call **c){
	if(!e)return;
	if(e->kind == EXPR_EXPR_LIST){
		arguments_solve(e->left,file,c);
		arguments_solve(e->right,file,c);
		return;
	}
	expr_codegen(e,file);
	if(!(*c)){
		(*c) = malloc(sizeof(*c));
		(*c)->reg = e->reg;
		(*c)->next = NULL;
	}
	else{
		struct call *c1 = malloc(sizeof(*c1));
		c1->reg = e->reg;
		c1->next = NULL;
		struct call *c2 = *c;
		while(c2->next!=NULL)c2 = c2->next;
		c2->next = c1; 
		
	}
	//register_free(e->reg);
	//arguments2++;
}

