	
#ifndef SCOPE_H
#define SCOPE_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include <stdio.h>
#include "symbol.h"
/*
struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};
*/
struct hash_list{
	struct hash_list *before;
	struct hash_table *h;// = hash_table_create(0,0);
	int param_count;
	int local_count;
	
};
void scope_enter();
void scope_leave();
int scope_level();
void scope_bind(char *name, struct symbol *s);
struct symbol *scope_lookup(const char *name);
struct symbol *scope_lookup_aux(const char *name);
struct symbol *scope_lookup_local(const char *name);



#endif


