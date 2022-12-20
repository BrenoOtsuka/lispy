#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <editline/readline.h>
#include <editline/history.h>

#include "lbuiltin.h"
#include "lval.h"
#include "lenv.h"
#include "mpc.h"

#include "commons/lerror.h"

mpc_parser_t* Comment;
mpc_parser_t* Number;
mpc_parser_t* String;
mpc_parser_t* Symbol;
mpc_parser_t* Sexpr;
mpc_parser_t* Qexpr;
mpc_parser_t* Expr;
mpc_parser_t* Lispy;

extern int errno;

lval*
builtin_load(lenv* e, lval* a) {
    LASSERT_NUM("load", a, 1);
    LASSERT_TYPE("load", a, 0, LVAL_STR);

    mpc_result_t r;
    if (mpc_parse_contents(a->cell[0]->str, Lispy, &r)) {

        lval* expr = lval_read(r.output);
        mpc_ast_delete(r.output);

        while(expr->count) {
            lval* x = lval_eval(e, lval_pop(expr, 0));

            if(x->type == LVAL_ERR) { lval_println(x); }
            lval_del(x);
        }

        lval_del(expr);
        lval_del(a);

        return lval_sexpr();
    } else {
        char* err_msg = mpc_err_string(r.error);
        mpc_err_delete(r.error);

        lval* err = lval_err("Coult not load Lybrary %s", err_msg);
        free(err_msg);
        lval_del(a);

        return err;
    }
}

int
main(int argc, char* argv[])
{
    puts("Lispy Version 0.0.1");
    puts("Press Ctrl+C to exit\n");

    Comment  = mpc_new("comment");
    Number   = mpc_new("number");
    String   = mpc_new("string");
    Symbol   = mpc_new("symbol");
    Sexpr    = mpc_new("sexpr");
    Qexpr    = mpc_new("qexpr");
    Expr     = mpc_new("expr");
    Lispy    = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "number : /-?[0-9]+/;                               \
         string : /\"(\\\\.|[^\"])*\"/;                     \
         symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/;         \
         comment: /;[^\\r\\n]*/;                            \
         sexpr  : '(' <expr>* ')';                          \
         qexpr  : '{' <expr>* '}';                          \
         expr   : <number>  | <symbol> | <string>           \
                | <comment> | <sexpr>  | <qexpr>;           \
         lispy  : /^/ <expr>* /$/;",
        Comment, Number, String, Symbol, Sexpr, Qexpr, Expr, Lispy
    );

    lenv* e = lenv_new();
    lenv_add_builtins(e);
    lenv_add_builtin(e, "load" , builtin_load);

    if (argc == 1) {
        while(1) {
            char* input = readline("lispy> ");
            
            add_history(input);

            mpc_result_t r;

            if (mpc_parse("<stdin>", input, Lispy, &r)) {

                lval* a = lval_read(r.output);
                lval* x = lval_eval(e, a);
                lval_println(x);
                lval_del(x);

                mpc_ast_delete(r.output);
            } else {

                mpc_err_print(r.error);
                mpc_err_delete(r.error);
            }
            free(input);
        }
    }

    if (argc >= 2) {
        for (int i = 1; i < argc; ++i) {

            lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));
            lval* x = builtin_load(e, args);

            if (x->type == LVAL_ERR) { lval_println(x); }

            lval_del(x);
        }
    }

    lenv_del(e);

    mpc_cleanup(8, Comment, Number, String, Symbol, Sexpr, Qexpr, Expr, Lispy);

    return 0;
}
