	
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include <stdio.h>

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next );
void decl_print( struct decl *d, int indent );
void decl_resolve(struct decl *d,int a);
void decl_typecheck(struct decl *d);
struct hash_table* getParams();
void expr_constant(struct expr* e);
int get_incrementError(int kind);
void resolve_array_values(struct expr *e,struct type *t,const char* name);
void decl_codegen(struct decl *d, FILE *file);
void incremet_local_params_variables();
int get_total_local_params_variables();
int get_total_function();
#endif


