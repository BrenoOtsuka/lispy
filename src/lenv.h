#ifndef __LENV_H__
#define __LENV_H__

struct lval;
struct lenv;

typedef struct lval lval;
typedef struct lenv lenv;

typedef lval* (*lbuiltin) (lenv*, lval*);

struct lenv {
    lenv* par;
    int count;
    char** syms;
    lval** vals;
};


lenv*
lenv_new(void);

lenv*
lenv_copy(lenv* e);

lval*
lenv_get(lenv* e, lval* k);

void
lenv_put(lenv* e, lval* k, lval* v);

void
lenv_def(lenv* e, lval* k, lval* v);

void
lenv_del(lenv* e);

void
lenv_add_builtin(lenv* e, char* name, lbuiltin func);

void
lenv_add_builtins(lenv* e);

#endif
