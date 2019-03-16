/* first version: read expression, creating tree
 * 4.12.2018
 */
#include <stdio.h>
#include "dif_tree.c"

const int MAX_SPUSK_FILE_SIZE = 100;
char *s = NULL;

node *getG (char *a, tree_t *equ);
node *getE (tree_t *equ);
node *getT (tree_t *equ);
node *getP (tree_t *equ);
node *getN (tree_t *equ);
node *getZ (tree_t *equ);
/*
int main ()
{
	tree_t *equ = (tree_t *) calloc (1, sizeof (*equ));
	tree_ctor (equ);
	
	FILE * file_in = fopen ("file_in.txt", "rb");
	if (file_in == NULL)
    {
        printf("Do not open file\n");
        return -1;
    }
	char *buf = (char *) calloc (MAX_SPUSK_FILE_SIZE, sizeof(*buf));
	fread (buf, MAX_SPUSK_FILE_SIZE, sizeof(*buf), file_in);
	
	node * rez = getG (buf, equ);
	//tree_print (equ);
	tree_koncole (equ->root);
	//tree_dot_print (equ);
	tree_t *dif_equ = (tree_t *) calloc (1, sizeof (*dif_equ));
	tree_ctor (dif_equ);
	dif_equ->root = tree_copy (equ->root);
	tree_dot_print (dif_equ);
	tree_dtor (equ);
	tree_dtor (dif_equ);
	return 0;
}
*/
node *getG (char * buf, tree_t *equ)
{
	s = buf;
	equ->root = getE (equ);
	//printf ("\nGval : %lg\n", equ->root->value);
	//assert (*s == '\n');
	s++;
	//assert (*s == '\0');
	return equ->root;
}

node *getN (tree_t *equ)
{
	//printf ("NNN%c", *s);
	//char *old_s = s;
	int val = 0;
	node *node_n = node_ctor ();
	while ('0' <= *s && *s <= '9')
	{
		val = 10*val + *s - '0';
		s++;
	}
	node_n->value = val;
	node_n->type = 'c';
	//printf ("Nval : %lg\n", node_n->value);
	//assert (old_s != s);
	return node_n;
}

node *getZ (tree_t *equ)
{
	node *node_z = node_ctor ();
	node_z->value = *s;
	node_z->type = 'v';
	s++;
	//printf ("\nZval : %lg\n", node_z->value);
	
	return node_z;
}
node *getE (tree_t *equ)
{
	node *node_t = getT (equ);
	node *node_e = NULL;
	
	while (*s == '+' || *s == '-')
	{
		char op = *s;
		s++;
		node_e = node_ctor ();
		node_e->type = 'o';
		node_e->value = op;
		
		node *node_t2 = getT(equ);
		node_new_add (equ, node_e, node_t, 'L');
		node_new_add (equ, node_e, node_t2, 'R');
		node_t = node_e;
	}
		//printf ("return from t to e\n");
	return node_t;
}

node *getT (tree_t *equ)
{
	node *node_p = getP (equ);
	node *node_t = NULL;
	
	while (*s == '*' || *s == '/')
	{
		char op = *s;
		s++;
		node_t = node_ctor ();
		node_t->type = 'o';
		node_t->value = op;
		
		node *node_p2 = getP(equ);
		node_new_add (equ, node_t, node_p, 'L');
		node_new_add (equ, node_t, node_p2, 'R');
		node_p = node_t;
	}
	//printf ("return from p to t\n");
	return node_p;
	
}

node *getP (tree_t *equ)
{
	if (*s == '(')
	{
		//printf ("(((((((\n");
		s++;
		node *node_p = getE (equ);
		//printf ("%c", *s);
		assert (*s == ')');
		s++;
		//printf ("in P: %s", s);
		return node_p;
	}
	else if (!(strncasecmp ("sin", s, 3)) || !(strncasecmp ("cos", s, 3))) /*в правой ветке выражение*/
	{
		double op = *s;
		s += 3;
		node *node_p = getP(equ);    
		return node_create ('o', op, NULL, node_p);
	}
	else if (!(strncasecmp ("ln", s, 2))) /*в правой ветке выражение*/
	{
		double op = *s;
		s += 2;
		node *node_p = getP(equ);    
		return node_create ('o', op, NULL, node_p);
	}
	else if (!(strncasecmp ("exp", s, 3))) /*в правой ветке выражение*/
	{
		double op = *s;
		s += 3;
		node *node_p = getP(equ);    
		return node_create ('o', op, NULL, node_p);
	}
	else if ('a' <= *s && *s <= 'z' && *s != 's' && *s != 'l' && *s != 'c' && *s != 'e')
	{
		node *node_z = getZ (equ);
		if (*s == '^') /*в левой ветке выражение, в правой степень*/
		{
			double op = *s;
			s++;
			node *node_p = getP(equ);    
			return node_create ('o', op, node_z, node_p);
		}
		return node_z;
	}
	else 
	{
		node *node_p = getN (equ);
		//printf ("1\n");
		return node_p;	
	}
}
