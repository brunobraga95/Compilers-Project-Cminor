#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"

//int error_count = 0;
typedef enum {
	EXPR_MINUS,
	EXPR_ARRAY_ELEMENT,
	EXPR_ARRAY_IDENT,	
	EXPR_ADD,
	EXPR_OPPAR,	
	EXPR_ASSIGN,
	EXPR_SUB,
	EXPR_MULT,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_NEG,
	EXPR_GT,
	EXPR_GE,
	EXPR_LT,
	EXPR_LE,
	EXPR_EQ,
	EXPR_NE,
	EXPR_INCREMENT,
	EXPR_DECREMENT,
	EXPR_OR,
	EXPR_AND,
	EXPR_EXPONENTIATION,
	EXPR_BOOLEAN_LITERAL,
	EXPR_NAME,
	EXPR_INTEGER_LITERAL,
	EXPR_CHAR_LITERAL,		
	EXPR_STRING_LITERAL,
	EXPR_EXPR_LIST,
	EXPR_UNARY_NEGATION,
	EXPR_ARRAY_LIST,
	EXPR_FUNC
	/* many more types to add here */
} expr_t;

struct expr {
	/* used by all expr types */
	expr_t kind;
	expr_t subkind;
	struct expr *left;
	struct expr *right;
	
	/* used by leaf expr types */
	const char *name;
	struct symbol *symbol;
	int literal_value;
	const char * string_literal;
	int reg;
};
struct call {
	struct call *next;
	int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_character_literal(const char *str );
struct expr * expr_create_string_literal( const char *str );

void expr_resolve(struct expr *e,int a);
void expr_print( struct expr *e );
struct type *expr_typecheck(struct expr *e);
void test_params_function(struct expr *e,struct param_list *p,const char *name);
void expr_codegen(struct expr *e, FILE *file);
void arguments_solve(struct expr *e,FILE *file,struct call **c);

#endif
