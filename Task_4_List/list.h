#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

enum list_error
{
	NO_ERROR = 0,           ///< The case of normal situation
    ALLOC_ERROR = 1,        ///< The case of Allocation failure
    NULL_POINTER_ERROR = 2, ///< The case of NULL pointer
    COUNT_ERROR,
    WRONG_POINTER_ERROR,
    REMOVE_NUM_ERROR,
    F_OPEN_ERROR
};

int LIST_ERROR = NO_ERROR;
enum poison
{
	COUNT_POISON = -1,
	POINTER_POISON = 0,
	VALUE_POISON = 666,
};
int FIND_NUM_COUNT = 0;
const int MAX_FIND_NUM_COUNT = 5;
typedef char * list_val;

typedef struct list_elem
{
	list_val value;
	struct list_elem *next;
	struct list_elem *prev;	
} list_elem;

typedef struct list_t
{
	//list_elem **data;
	list_elem *head;
	list_elem *tail;
	int count;
} list_t;

int list_ctor (list_t *s);
list_elem * list_elem_ctor ();

int list_dtor (list_t *s);
int list_elem_dtor (list_elem *s);

int list_clean (list_t *s);

list_elem * list_add_front (list_t *s, list_val value);
list_elem * list_add_back (list_t *s, list_val value);

list_val list_remove_front (list_t *s);
list_val list_remove_back (list_t *s);

list_elem * list_find_num (list_t *s, int num);
list_elem * list_find_val (list_t *s, list_val val);

list_val list_remove (list_t *s, list_elem * cur_elem);
list_elem * list_add_after (list_t *s, list_elem *cur_elem, list_val val);
list_elem * list_add_before (list_t *s, list_elem *cur_elem, list_val value);

int is_list_elem_ok (list_elem *s_elem);
void list_elem_dump (list_elem *s_elem);
void list_print (list_t *s);
int is_list_ok (list_t *s);

int list_dot_print (list_t *s);
