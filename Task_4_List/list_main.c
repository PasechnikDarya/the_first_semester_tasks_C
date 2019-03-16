#include "list_1_1.c"

int main ()
{
	list_t l = {0};
	list_ctor (&l);
	list_add_front (&l, 1);
	list_elem *a = list_add_back (&l, 2);
	list_remove_back (&l);
	list_add_back (&l, 3);
	list_print (&l);
	a = list_add_after (&l, a, 100);
	list_print (&l);
	list_add_back (&l, 4);
	list_print (&l);
	list_remove (&l,a);
	list_print (&l);
	list_add_front (&l, 0);
	list_find_num (&l,1);
	list_find_num (&l,1);
	list_find_num (&l,1);
	list_find_num (&l,1);
	list_find_num (&l,1);
	list_find_num (&l,1);
	list_print (&l);
	list_dot_print (&l);	
	
	return 0;
}

