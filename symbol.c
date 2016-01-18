#include "symbol.h"
#include "decl.h"
#include "expr.h"
#include "type.h"
#include "register.h"
#include "scope.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name, int qual,struct type *subtipo){
	struct symbol *s = malloc(sizeof(*s));
	s->kind = kind;
	s->type = type;
	s->name = name;
	s->which= qual;
	s->subtipo = subtipo;
	s->param_local_variables  = get_total_local_params_variables();
	return s;
}

const char* symbol_code(struct symbol *s){	
	char retorno[100];
	char *resp = malloc(256);
	int aux[30];
	if(s->kind ==  SYMBOL_GLOBAL)return s->name;
	int which  = s->param_local_variables*8;
	if(which > 0){
		retorno[0] = '-';
		int aux_ = which;
		int pos = 0;
		while(aux_!=0){
			aux[pos++] = aux_%10;
			aux_ = aux_/10;
		}
		pos--;
		int i,j = 1;
		for(i = pos;i>=0;i--){
			retorno[j] = 48+aux[i];
			j++;
		}
		retorno[j++] = '(';
		retorno[j++] = '%';
		retorno[j++] = 'r';
		retorno[j++] = 'b';
		retorno[j++] = 'p';
		retorno[j++] = ')';
		retorno[j] = '\0';
		strcpy(resp,retorno);
		return resp;
		
	}
	return "";
}









