//
// Created by penguin on 20.11.18.
//
#ifndef C_TREE_HEAD_H
#define C_TREE_HEAD_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
 
#define DEBUG

#ifdef DEBUG

#define assert_tree(s)                                                      \
{                                                                           \
    if ( (is_tree_ok(s)) )                                                 \
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

const int MAX_FILE_SIZE = 100;
const int MAX_LINE_SIZE = 30;

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

int TREE_ERROR = NO_ERROR;


const int COUNT_POISON = -1;
enum poison
{
	POINTER_POISON = 0,
	VALUE_POISON = 666,
};

typedef char tree_val;

typedef struct node
{
    tree_val *value;
    int type;
    struct node *right;
    struct node *left;
    struct node *parent;
} node;

typedef struct tree_t
{
    //list_elem **data;
    node *root;

    int count;
} tree_t;

int tree_ctor (tree_t *s);
node * node_ctor ();

int tree_dtor (tree_t *s);
int node_dtor (node *s);

int tree_clean (tree_t *s, node *tmp_elem);

node *insert_new_node (tree_t *s, tree_val value);
node *node_add (tree_t *s, node *prev, tree_val *value, char pos);

/*
tree_elem * tree_add_front (tree_t *s, tree_val value);
tree_elem * tree_add_back (tree_t *s, tree_val value);

tree_val tree_remove_front (tree_t *s);
tree_val tree_remove_back (tree_t *s);

tree_elem * tree_find_num (tree_t *s, int num);
tree_elem * tree_find_val (tree_t *s, tree_val val);

tree_val tree_remove (tree_t *s, int num);
tree_elem * tree_add_after (tree_t *s, int num, tree_val val);
tree_elem * tree_add_before (tree_t *s, int num, tree_val value);
*/
node * node_read (tree_t *s, int *i, char *buf, node * parent_node);
void tree_read (tree_t *s, FILE * file_in);

void node_print (node *tmp_elem);
void node_write(node *s, FILE *file_out);

void tree_print (tree_t *s);
int is_tree_ok (tree_t *s);
void is_node_ok (node *s);

void node_dump (node *s);
void tree_dump (node *s);

int tree_dot_print (tree_t *s);

#endif //C_TREE_HEAD_H
