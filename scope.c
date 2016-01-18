#include "symbol.h"
#include "scope.h"
#include "hash_table.h"
#include<stdio.h>
#include<stdlib.h>
struct hash_list *tail = NULL;
struct hash_list *tail_aux = NULL;
struct hash_list *head = NULL;
void scope_enter(){
        struct hash_list *x = malloc(sizeof(*x));
	x->param_count = 0;
	x->local_count = 0;
	x->h = hash_table_create(0,0); 
	if(!tail){
		tail = x;
		tail->before = NULL;
		tail_aux = tail;
	}
	else{
		x->local_count = tail->local_count;
		x->param_count = tail->param_count;
	   	x->before = tail;
		tail = x;
		tail_aux = tail;
	}
}

void scope_leave(){
	struct hash_list *aux = tail;
	tail = tail->before;
	free(aux);
}

int scope_level(){
    int c = 0;
    struct hash_list *aux = tail;
    while(aux){
    	aux=aux->before;
	c++;
     }	
     return c;	
}
void scope_bind(char *name, struct symbol *s){
	if(s->kind == SYMBOL_LOCAL)s->which = ++tail->local_count;
	if(s->kind == SYMBOL_PARAM)s->which = ++tail->param_count;
	hash_table_insert(tail->h,name,s);
	//hash_table_insert(tail_aux->h,name,s);
    
}

struct symbol *scope_lookup(const char *name){
    struct hash_list *aux = tail;
    struct symbol *sy;	
    while(aux){
	sy = hash_table_lookup(aux->h,name);
    	if(sy!=NULL){
		//type_print(sy->type);
		return sy;
	}
	aux = aux->before;
	}
     return NULL;
} 
struct symbol *scope_lookup_aux(const char *name){
    struct hash_list *aux = tail_aux;
    struct symbol *sy;	
    while(aux){
	sy = hash_table_lookup(aux->h,name);
    	if(sy!=NULL){
		//type_print(sy->type);
		return sy;
	}
	aux = aux->before;
	}
     return NULL;
} 


struct symbol *scope_lookup_local(const char *name){
    if(tail == NULL)return NULL;
    if(hash_table_lookup(tail->h,name)!=NULL)return hash_table_lookup(tail->h,name); 
    return NULL;
} 


