#ifndef UNTITLED_DASHAAA_H
#define UNTITLED_DASHAAA_H


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define DEBUG

#ifdef DEBUG

#define assert_tree(s)                                                      \
{                                                                           \
    if ( (is_tree_ok(s)) )                                                  \
    {                                                                       \
        printf ("\n----------------------------------------------\n");      \
        printf (" \t* TREE_ERROR %d\n"                                      \
                " \t* file       %s\n"                                      \
                " \t* line       %d\n"                                      \
                " \t* function   %s\n"                                      \
                , TREE_ERROR, __FILE__, __LINE__, __FUNCTION__);            \
        printf("\n----------------------------------------------\n");       \
        abort();                                                            \
    }                                                                       \
}

#else

#define assert_tree(s) ;

#endif

int count_func = 0;
int var_pos = 0;
int ff = 0;

enum
{
    type_CONST = 'c',
    type_VAR = 'v',  ///< переменные
    type_OP = 'o', ///< оператор
    type_F = 'F',  ///< объявление функции
    type_NG = 'b', ///< начало программы
    type_P = 'P', ///< блоки исполнения
    type_Ex = 'X', ///< конец программы
    type_f = 'f',  ///< вызов функции
    type_d = 'd', ///< число локальных переменных
    type_v = 'V', ///< объявление параметров функции
    type_p = 'p',  ///< параметры функции
    type_u = 'u'  ///< параметры функции
};

enum tree_error
{
    NO_ERROR,           ///< The case of normal situation
    ALLOC_ERROR ,        ///< The case of Allocation failure
    NULL_POINTER_ERROR, ///< The case of NULL pointer
    COUNT_ERROR,
    WRONG_POINTER_ERROR,
    REMOVE_NUM_ERROR,
    F_OPEN_ERROR
};


enum poison
{
    POINTER_POISON = 0,
    VALUE_POISON = 666,
};

typedef struct func
{
    char *name;
    int count_params; //кол-во параметров
    int all_stuff_count; //кол-во параметров
    int point;
} func;

typedef struct vari
{
    char name;
    int pos;
} vari;


vari *varies[500];
vari *f_varies[10];



typedef double tree_val;

typedef struct node
{
    tree_val value;
    int type;
    struct node *right;
    struct node *left;
    struct node *parent;
} node;

typedef struct tree_t
{
    node *root;
    int count;
} tree_t;



//node *getG (char *a, tree_t *equ);
node *getE (tree_t *equ, func * f_mem);
node *getT (tree_t *equ, func * f_mem);
node *getP (tree_t *equ, func * f_mem);
node *getN (tree_t *equ);
node *getZ (tree_t *equ);
node *getA (tree_t *equ, func * f_mem); //prisvaivanie A ::= Z '=' [E || I]           oke
node *getIf (tree_t *equ, func * f_mem); //if If::= if (C) [Op, M];                   oke
node *getOp (tree_t *equ, func * f_mem); //operator Op ::= (A|If|W|F|I|O)';'          oke
node *getM (tree_t *equ, func * f_mem); //operators M ::= '{'[Op]*'}'                 oke
node *getNG (char * buf, tree_t *equ, func * f_mem); //main G::= [Op]*'end'           oke
node *getF (tree_t *equ, func * f_mem); //func F ::= 'T-T' Id '(' ')'{}
node *getFu (tree_t *equ, func * f_mem); //func Fu ::= 'F'('x, y, ...)
node *getW (tree_t *equ, func * f_mem); //for W ::= 'while' (C) [op || M]             oke
node *getI (tree_t *equ, func * f_mem); //input I ::= U.U                             oke
node *getO (tree_t *equ, func * f_mem); //output O ::= 0_0 (E)                        oke
node *getC (tree_t *equ, func * f_mem); //compare C ::= E ['O_O', 'o_O', 'O_o'] E     oke
node *getR (tree_t *equ, func * f_mem); //compare R ::= 'x, ...'                      oke




int tree_ctor (tree_t *s);
node * node_ctor ();

int tree_dtor (tree_t *s);
int node_dtor (node *s);

int tree_clean (tree_t *s, node *tmp_elem);

node * node_read (tree_t *s, int *i, char *buf, node * parent_node);
void tree_read (tree_t *s, FILE * file_in);

void node_print (node *tmp_elem);
void node_write(node *s, FILE *file_out);

void tree_print (tree_t *s);
int is_tree_ok (tree_t *s);
void is_node_ok (node *s);

void node_dump (node *s);
void tree_dump (node *s);

void tree_dot_print (tree_t *s);
void create_tree (node *s, FILE *f);
node *node_create (char type, double val, node *lefty, node *righty);
void print_type(node* pos, FILE* dot_out);
node *zero_node (node * cur);

node *node_new_add (tree_t *s, node *prev, node *elem, char pos);
void tree_koncole (node* s);
//----------------------------------------------------------------------
void node_asmer (FILE *file_out, node * root, func *f_mem);
void tree_asmer (char * file, tree_t *equ, func *f_mem);

int find_fcount_params(char *name, func *f_mem);
int num_func(char *name, func *f_mem);
int find_var(char name);
int find_Fvar(char name);



#endif //UNTITLED_DASHAAA_H
