#ifndef __LVAL_H__
#define __LVAL_H__

#include "mpc.h"

struct lval;
struct lenv;

typedef struct lval lval;
typedef struct lenv lenv;

typedef lval* (*lbuiltin) (lenv*, lval*);

struct lval {
    int type;
    // Basic
    long num;
    char* err;
    char* str;
    char* sym;
    // Function
    lbuiltin builtin;
    lenv* env;
    lval* formals;
    lval* body;
    // Expression
    int count;
    struct lval** cell;
};

enum { LVAL_ERR, LVAL_NUM, LVAL_STR, LVAL_SYM, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

lval*
lval_num(long x);

lval*
lval_str(char* s);

lval*
lval_err(char* fmt, ...);

lval*
lval_sym(char* s);

lval*
lval_fun(lbuiltin func);

lval*
lval_lambda(lval* formals, lval* body);

lval*
lval_sexpr(void);

lval*
lval_qexpr(void);

int
lval_eq(lval* x, lval* y);

lval*
lval_add(lval* v, lval* x);

void
lval_del(lval* v);

lval*
lval_copy(lval* v);

lval*
lval_read_num(mpc_ast_t* t);

lval*
lval_read_str(mpc_ast_t* t);

lval*
lval_read(mpc_ast_t* t);

char*
ltype_name(int t);

void
lval_print_expr(lval* v, char open, char close);

void
lval_print_str(lval* v);

void
lval_print(lval* v);

void
lval_println(lval* v);

lval*
lval_pop(lval* v, int i);

lval*
lval_take(lval* v, int i);

lval*
lval_join(lval* x, lval* y);

lval*
lval_eval_sexpr(lenv* e, lval* v);

lval*
lval_eval(lenv* e, lval* v);

lval*
lval_call(lenv* e, lval* f, lval* a);

#endif
