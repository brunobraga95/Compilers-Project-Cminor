#include "param_list.h"
#include "type.h"
#include "scope.h"
#include <stdlib.h>
#include <stdio.h>


struct param_list * param_list_create( char *name, struct type *type, struct param_list *next )
{
	struct param_list *t = malloc(sizeof(*t));
	t->name = name;
	t->type = type;
	t->next = next;
	return t;

}
void param_list_print( struct param_list *a )
{
	if(!a)return;
	printf(" %s: ",a->name);
	type_print(a->type);
	if(a->next!=NULL)printf(",");
	param_list_print(a->next);
}

void param_list_resolve(struct param_list *p,int n)
{
	if(!p)return;
	incremet_local_params_variables();
	struct symbol *sym = symbol_create(SYMBOL_PARAM,p->type,p->name,n,0);
	scope_bind(p->name,sym);
	param_list_resolve(p->next,n+1);
}