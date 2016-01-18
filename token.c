#include "token.h"
char* token_string(token_t t)
{
  switch(t)
  {
    case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
    case TOKEN_STRING: return "STRING";
    case TOKEN_WHILE: return "WHILE";
    case TOKEN_ELSE: return "ELSE";
    case TOKEN_ARRAY: return "ARRAY";
    case TOKEN_IF: return "IF";
    case TOKEN_INTEGER_LITERAL: return "INTEGER_LITERAL";
    case TOKEN_CHARACTER_LITERAL: return "CHARACTER_LITERAL";
    case TOKEN_GE: return "GE";
    case TOKEN_SCOLON: return "SEMICOLON";
    case TOKEN_IDENTIFIER: return "IDENTIFIER";
    case TOKEN_LE: return "LE";
    case TOKEN_GT: return "GT";
    case TOKEN_LT: return "LT";
    case TOKEN_OR: return "OR";
    case TOKEN_AND: return "AND";
    case TOKEN_NOT: return "NOT";
    case TOKEN_EQ: return "EQ";
    case TOKEN_NE: return "NE";
    case TOKEN_ADD: return "PLUS";
    case TOKEN_SUB: return "MINUS";
    case TOKEN_MULT: return "MULT";
    case TOKEN_DIV: return "DIV";
    case TOKEN_MOD: return "MOD";
    case TOKEN_NEG: return "NEG";
    case TOKEN_OPPAR: return "OPPAR";
    case TOKEN_CLPAR: return "CLPAR";
    case TOKEN_OPBRAK: return "OPBRAK";
    case TOKEN_CLBRAK: return "CLBRAK";
    case TOKEN_OPBRACE: return "OPBRACE";
    case TOKEN_CLBRACE: return "CLBRACE";
    case TOKEN_ASSIGN: return "ASSIGN";
    case TOKEN_INTEGER: return "INTEGER";
    case TOKEN_CHAR: return "CHAR";
    case TOKEN_DO: return "DO";
    case TOKEN_FALSE: return "FALSE";
    case TOKEN_VOID: return "VOID";
    case TOKEN_TRUE: return "TRUE";
    case TOKEN_FUNCTION: return "FUNCTION";
    case TOKEN_FOR: return "FOR";
    case TOKEN_PRINT: return "PRINT";
    case TOKEN_BOOLEAN: return "BOOLEAN";
    case TOKEN_RETURN: return "RETURN";
    case TOKEN_INCREMENT: return "INCREMENT";
    case TOKEN_DECREMENT: return "DECREMENT";
    case TOKEN_EXPONENTIATION: return "EXPONENTIATION";
    case TOKEN_COLON: return "COLON";
    case TOKEN_COMMA: return "COMMA";
    case TOKEN_ERROR: return "scan error: ";
    default: return "error";
  }


}
