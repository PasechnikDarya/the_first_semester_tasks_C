#include "list.h"

#define DEBUG

#ifdef DEBUG

#define ASSERT_OK(s)                                                    \
	if (is_list_ok(s))                                                  \
	{                                                                   \
		list_dump(*(s));                                                \
	}
	
#define list_dump(s)                                                    \
{                                                                       \
	printf ("\n----------------------------------------------\n");      \
	printf (" \t* LIST_ERROR %d\n"                                      \
			" \t* file       %s\n"                                      \
			" \t* line       %d\n"                                      \
			" \t* function   %s\n"                                      \
			, LIST_ERROR, __FILE__, __LINE__, __FUNCTION__);            \
	printf("\n----------------------------------------------\n");       \
	if (LIST_ERROR != NULL_POINTER_ERROR && LIST_ERROR != ALLOC_ERROR)  \
	{                                                                   \
		list_print (&(s));                                              \
		exit (LIST_ERROR);                                              \
	}                                                                   \
	else   exit (LIST_ERROR);                                                                                                                                \
}                                                                       


#else

#define list_dump(s) ;
#define ASSERT_OK(s) ;

#endif

int list_ctor (list_t *s)
{
	assert(s);
	
	//s->data = (list_elem **) calloc (START_LIST_SIZE, sizeof(size_elem*));
	s->tail = NULL;
	s->head = NULL;
	s->count = 0;
	
	return LIST_ERROR;
}

list_elem *list_elem_ctor ()
{	
	list_elem *s = (list_elem*) calloc(1, sizeof(*s));
	if (s == NULL)
	{
		LIST_ERROR = ALLOC_ERROR;
		return NULL;
	}
	
	s->value = (list_val) calloc (1, sizeof(s->value));
	if (s->value == 666)
	{
		LIST_ERROR = ALLOC_ERROR;
	}
	
	s->next = NULL;
	
	s->prev = NULL;

	return s;
}
//----------------------------------------------------------------------
int list_dtor (list_t *s)
{
	ASSERT_OK(s);
	list_clean (s);

	//free (data);
	s->head = NULL;
	s->tail = NULL;
	s->count = COUNT_POISON;

	return LIST_ERROR;
}

int list_elem_dtor (list_elem *s)
{
	assert(s);
	
	s->value = VALUE_POISON;
	free (&(s->value));

	free (s);
	s = POINTER_POISON;
	
	return LIST_ERROR;
}
//----------------------------------------------------------------------
int list_clean (list_t *s)
{
	ASSERT_OK(s);
	
	list_elem * tmp_elem = s->head;
	for (int i = 0; i < s->count; i++)
	{
		tmp_elem = tmp_elem->next;
		list_elem_dtor(tmp_elem->prev);
	}
	
	ASSERT_OK(s);
	
	return LIST_ERROR;
}

list_elem *list_add_front (list_t *s, list_val value)
{
	ASSERT_OK(s);
	
	list_elem *s_elem = list_elem_ctor();
	s_elem->value = value;
	
	if (s->count == 0)	
	{
		s_elem->next  = NULL;
		s_elem->prev  = NULL;
		s->head = s_elem;
		s->tail = s_elem;
	}
	else
	{
		s->head->prev = s_elem;
		s_elem->next = s->head;
		s->head = s_elem;
		s_elem->prev = NULL;
	}
	
	s->count++;
	
	ASSERT_OK(s);
	
	return s_elem;
}

list_elem *list_add_back (list_t *s, list_val value)
{
	ASSERT_OK(s);
	
	list_elem *s_elem = list_elem_ctor();
	
	s_elem->value = value;
	
	if (s->count == 0)	
	{
		s_elem->next  = NULL;
		s_elem->prev  = NULL;
		s->head = s_elem;
		s->tail = s_elem;
	}
	else
	{	
		s->tail->next = s_elem;
		s_elem->prev = s->tail;
		s->tail = s_elem;
		s_elem->next = NULL;
	}
	
	
	
	ASSERT_OK(s);
	s->count++;
	
	return s_elem;
}
//----------------------------------------------------------------------
list_elem * list_add_after (list_t *s, list_elem *cur_elem, list_val value)
{
	ASSERT_OK(s);
	
	list_elem *s_elem = list_elem_ctor ();
	
	if (cur_elem->next == NULL)
	{
		list_add_back (s, value);
	}
	else
	{
		s_elem->value = value;
		cur_elem->next->prev = s_elem;
		s_elem->next = cur_elem->next;
		s_elem->prev = cur_elem;
		cur_elem->next = s_elem;
		s->count++;
	}
	//printf ("\n val: %d, cur: %p, next: %p, prev^ %p", s_elem->value, s_elem, s_elem->next, s_elem->prev);

	
	ASSERT_OK(s);
	return s_elem;
}

list_elem * list_add_before (list_t *s, list_elem *cur_elem, list_val value)
{
	ASSERT_OK(s);
	
	list_elem *s_elem = list_elem_ctor ();
	
	if (cur_elem->prev == NULL)
	{
		list_add_front (s, value);
	}
	else
	{
		s_elem->value = value;
		cur_elem->prev->next = s_elem;
		s_elem->prev = cur_elem->prev;
		s_elem->next = cur_elem;
		cur_elem->prev = s_elem;
		s->count++;
	}
	
	//printf ("\n val: %d, cur: %p, next: %p, prev^ %p", s_elem->value, s_elem, s_elem->next, s_elem->prev);
		
	
	ASSERT_OK(s);
	return s_elem;
}
//----------------------------------------------------------------------
list_val list_remove_back (list_t *s)
{
	ASSERT_OK(s);
	
	list_elem *last_elem = s->tail;
	list_val last_val = last_elem->value;
	
	s->tail = last_elem->prev;
	s->tail->next = NULL;
	list_elem_dtor (last_elem);
	s->count--;
	
	ASSERT_OK(s);
	return last_val;
}

list_val list_remove_front (list_t *s)
{
	ASSERT_OK(s);
	
	list_elem *first_elem = s->head;
	list_val first_val = first_elem->value;
	
	s->head = first_elem->next;
	s->head->prev = NULL;
	list_elem_dtor (first_elem);
	s->count--;
	ASSERT_OK(s);
	
	return first_val;
}

list_val list_remove (list_t *s, list_elem *cur_elem)
{
	ASSERT_OK(s);
	
	list_val cur_val = cur_elem->value;
	
	if (cur_elem->prev == NULL)
	{
		list_remove_front (s);
	}
	else if (cur_elem->next == NULL)
	{
		list_remove_back (s);
	}
	else
	{
		cur_elem->prev->next = cur_elem->next;
		cur_elem->next->prev = cur_elem->prev;
		s->count--;
	}
	
	list_elem_dtor (cur_elem);
	
	
	ASSERT_OK(s);
	return cur_val;
}
//----------------------------------------------------------------------
list_elem * list_find_num (list_t *s, int num)
{
	ASSERT_OK(s);
	
	if (FIND_NUM_COUNT > MAX_FIND_NUM_COUNT)
	{
		FIND_NUM_COUNT++;
		
		int a = rand() % s->count;
		list_elem * tmp_elem = s->head;
		for (int i = 0; i < a; i++)
		{
			tmp_elem = tmp_elem->next;
		}
		return tmp_elem;
	}
	
	printf ("Do you really want to do it?\n");
	
	if (FIND_NUM_COUNT == MAX_FIND_NUM_COUNT)
	{
		printf ("It is last time when this function works correctly\n");
	}
	else if (FIND_NUM_COUNT < MAX_FIND_NUM_COUNT)
	{
		printf ("There are %d more not used attempts\n", MAX_FIND_NUM_COUNT - FIND_NUM_COUNT);
	}
	
	if (num >= s->count)
	{
		LIST_ERROR = REMOVE_NUM_ERROR;
		ASSERT_OK(s);
		return NULL;
	}
	
	list_elem * tmp_elem = s->head;
	for (int i = 0; i < num; i++)
	{
		tmp_elem = tmp_elem->next;
	}
	
	ASSERT_OK(s);
	
	FIND_NUM_COUNT++;
	return tmp_elem;
}

list_elem * list_find_val (list_t *s, list_val val)
{
	ASSERT_OK(s);
	
	list_elem * tmp_elem = s->head;
	while (tmp_elem->value != val)
	{
		tmp_elem = tmp_elem->next;
	}
	
	ASSERT_OK(s);
	return tmp_elem;
}
//----------------------------------------------------------------------
int is_list_ok (list_t *s)
{
	assert(s);
	if (LIST_ERROR != NO_ERROR)
	{
		return LIST_ERROR;
	}
	if (s->count < 0)
	{
		return LIST_ERROR = COUNT_ERROR;
		printf("ERROR COUNT\n");
	}

	if (s->count != 0 && s->head == NULL )
	{
		return LIST_ERROR = NULL_POINTER_ERROR;
		printf("NULL_POINTER_ERROR\n");
	}

	if (s->count != 0 && s->tail == NULL)
	{
		return LIST_ERROR = NULL_POINTER_ERROR;
		printf("NULL_POINTER_ERROR\n");
	}
	if (s->count != 0)
	{
		
		list_elem *tmp_elem = s->head->next;
		
		for (int i = 1; i < s->count; i++)
		{
			if (tmp_elem->prev != NULL && tmp_elem->next != NULL)
			{
				if (tmp_elem->prev->next != tmp_elem->next->prev) return LIST_ERROR = WRONG_POINTER_ERROR;
				else tmp_elem = tmp_elem->next;
			}
			else if (tmp_elem->prev == NULL && tmp_elem != s->head)
			{
				return LIST_ERROR = WRONG_POINTER_ERROR;
			}
			else if (tmp_elem->next == NULL && tmp_elem != s->tail)
			{
				return LIST_ERROR = WRONG_POINTER_ERROR;
			}
		}
	}
	return LIST_ERROR;
}

int is_list_elem_ok (list_elem *s_elem)
{
	if (LIST_ERROR != NO_ERROR)		return LIST_ERROR;
	
	if (s_elem == NULL)				return LIST_ERROR = NULL_POINTER_ERROR;

	return LIST_ERROR;
}

void list_elem_dump (list_elem *s_elem)
{
	if (is_list_elem_ok (s_elem) != NO_ERROR)	printf ("%d", LIST_ERROR);
	
	assert (s_elem);
	printf ("%5d : cur : %15p : next : %15p : prev : %15p\n", s_elem->value, s_elem, s_elem->next, s_elem->prev);
	
	return;
}

void list_print (list_t *s)
{
	printf ("\n # list %p head %8p tail %8p count %3d\n", s, s->head, s->tail, s->count);
	list_elem * tmp_elem = s->head;
	for (int i = 0; i < s->count; i++)
	{
		list_elem_dump (tmp_elem);
		tmp_elem = tmp_elem->next;
	}
}
int list_dot_print (list_t *s)
{
	FILE *file_dot = fopen ("file_dot.dot", "w");
	if (file_dot == NULL)
	{
		printf ("Opening file failure\n");
		LIST_ERROR = F_OPEN_ERROR;

		return LIST_ERROR;
	}
	
	fprintf (file_dot, "\ndigraph G{\n");
	fprintf (file_dot,  "node[color=\"green\",fontsize=10, fillcolor=\"lightgrey\"];\n");
	fprintf (file_dot,  "edge[color=\"darkgreen\",fontsize=12];\n");
	fprintf (file_dot,  "rankdir=HR;\n");
	list_elem *tmp_elem = s->head;
	for (int i = 0; i < s->count; i++)
	{
		if (tmp_elem->next != NULL)
		{
			
			fprintf (file_dot, "elem%d [shape = record,label = \"  <value> value:\\n%d | <cur> address:\\n%p| <next> next:\\n%p | <prev> prev:\\n%p\"];\n", i, tmp_elem->value, tmp_elem, tmp_elem->next, tmp_elem->prev);
			fprintf (file_dot, "elem%d [shape = record,label = \"  <value> value:\\n%d | <cur> address:\\n%p | <next> next:\\n%p | <prev> prev:\\n%p\"];\n", i+1, tmp_elem->next->value, tmp_elem->next, tmp_elem->next->next, tmp_elem->next->prev);
			fprintf (file_dot, "elem%d:<next> -> elem%d:<cur>;\n", i, i+1);
			
			tmp_elem = tmp_elem->next;
		}
	}
	tmp_elem = s->tail;
	
	for (int i = s->count-1; i >= 0 ; i--)
	{
		if (tmp_elem->prev != NULL)
		{
			fprintf (file_dot, "elem%d:<prev> -> elem%d:<cur>[style=\"dashed\"];\n", i, i-1);
			tmp_elem = tmp_elem->prev;
		}
	}
	
	fprintf (file_dot, "}\n");
	
	fclose (file_dot);
	/*
	system ("-Tpng file_dot.dot -o list_dot");
	system ("./list_dot");
	*/
	return LIST_ERROR;
}
