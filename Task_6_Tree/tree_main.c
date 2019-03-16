#include "tree.c"

int main()
{
	FILE * file_in = fopen ("file_in.txt", "r");
	tree_t s;
	tree_ctor (&s);
	tree_read (&s, file_in);
	tree_print (&s);
	insert_new_node (&s, 10);
	tree_print (&s);
	tree_write (&s, "file_out.txt");
	tree_dtor (&s);	
    printf ("H");
    return 0;
}
