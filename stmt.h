 	
#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "expr.h"

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK,
	STMT_SCOLON
} stmt_kind_t;

struct stmt {
	stmt_kind_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
	int if_cont_aux;
	int for_cont_aux;	
};

struct stmt * stmt_create( stmt_kind_t kind, struct decl *d, struct expr *init_expr, struct expr *e, struct expr *next_expr, struct stmt *body, struct stmt *else_body,struct stmt *next);
void stmt_print( struct stmt *s, int indent );
void identar(int i);
void stmt_resolve(struct stmt *s,int a);
void stmt_typecheck(struct stmt *s,struct type *tipo);
void stmt_codegen(struct stmt *s,FILE *file);
void stmt_printout(struct expr *e,FILE *file);

#endif
