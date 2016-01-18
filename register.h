	
#ifndef REGISTER_H
#define REGISTER_H
/*
#include "type.h"
#include "stmt.h"
#include "expr.h"
#include <stdio.h>
#include "symbol.h"
*/
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

const char *register_name(int r);
int register_alloc();
void register_free(int r);



#endif


