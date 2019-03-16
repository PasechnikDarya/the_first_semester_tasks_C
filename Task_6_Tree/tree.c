//
// Created by penguin on 20.11.18.
//

#include "tree_head.h"

node *insert_new_node (tree_t *s, tree_val value)
{
	s->count++;
    node *new_node = node_ctor ();
    node *tmp_node = NULL;
    *new_node->value = value;

    if (s->root == NULL) return s->root = new_node;

	tmp_node = s->root;
	while (tmp_node)
	{
		if (value > *tmp_node->value)
        {
		    if (tmp_node->right)    tmp_node = tmp_node->right;
		    else
            {
		        tmp_node->right = new_node;
		        new_node->parent = tmp_node;

		        return new_node;
            }
        }
        else
        {
            if (tmp_node->left)     tmp_node = tmp_node->left;
            else
            {
                tmp_node->left = new_node;
                new_node->parent = tmp_node;

                return new_node;
            }
        }
	}
	return NULL;
}

node *node_add (tree_t *s, node *prev, tree_val *val, char pos)
{
	if (pos == 'L')
	{
        node *elem = node_ctor();
        elem->value = val;

        elem->parent = prev;
        prev->left = elem;
        s->count++;

        assert_tree(s);
        return elem;
    }

    if (pos == 'R')
	{
        node *elem = node_ctor();
        elem->value = val;

        elem->parent = prev;
        prev->right = elem;
        s->count++;

        assert_tree(s);
        return elem;
    }

    assert_tree(s);
    return NULL;
}
//for differentiator: add new node to ... and reolace values
node *node_smart_add (tree_t *s, node *prev, node *elem, char pos)
{
	if (pos == 'L')
	{
        elem->parent = prev->parent;
        
        if (elem->parent->left == prev)		prev->parent->left = elem;
		else 								prev->parent->right = elem;
        
        prev->parent = elem;
        elem->left = prev;
        s->count++;

        assert_tree(s);
        return elem;
    }

    if (pos == 'R')
	{
        elem->parent = prev->parent;
        
        if (elem->parent->left == prev)		prev->parent->left = elem;
		else 								prev->parent->right = elem;
        
        prev->parent = elem;
        elem->right = prev;
        s->count++;

        assert_tree(s);
        return elem;
    }

    assert_tree(s);
    return NULL;
}


node *node_new_add (tree_t *s, node *prev, node *elem, char pos)
{
	if (pos == 'L')
	{
        elem->parent = prev;
        prev->left = elem;
        s->count++;

        assert_tree(s);
        return elem;
    }

    if (pos == 'R')
	{
        elem->parent = prev;
        prev->right = elem;
        s->count++;

        assert_tree(s);
        return elem;
    }

    assert_tree(s);
    return NULL;
}

void tree_write (tree_t *s, char *file)
{
	FILE *file_out = fopen (file, "w");
	node_write (s->root, file_out);
	fclose (file_out);
}
void node_write (node *n_node, FILE *file_out)
{
    if (!n_node) return;
    fprintf (file_out, "(");
    
    if (n_node->left != NULL)
    {
		node_write (n_node->left, file_out);
	}
    fprintf (file_out, "%d", *n_node->value);
	if (n_node->right != NULL)
    {
		node_write (n_node->right, file_out);
	}
	fprintf (file_out, ")");   

    return;
}

void tree_read (tree_t *s, FILE * file_in)
{
    char *buf = (char *) calloc (MAX_FILE_SIZE, sizeof(*buf));
    fread (buf, sizeof(double), MAX_FILE_SIZE, file_in);
    /*
    fseek (file_in, 0, SEEK_END);
    int s_count = ftell (file_in);
    fseek (file_in, 0, SEEK_SET);
    */

    int i = 0;

    s->root = node_read (s, &i, buf, s->root);
    
    //printf ("i finished reading %d\n", i);
    return;
}

node * node_read (tree_t *s, int *i, char *buf, node * parent_node)
{
    //assert_tree (s);
    assert (i);
    assert (buf);

    node * new_node = node_ctor();
    new_node->parent = parent_node;
    s->count++;
    
    //printf ("i read nodes %d\n", *i);
    
    if ('(' == buf[*i])
    {
		(*i)++;
	    //printf ("i found node %d\n", *i);			 
        if ('(' == buf[*i])
        {
			//printf ("i found left node %d\n", *i);
            new_node->left = node_read (s, i, buf, new_node);
            //printf ("oooooookey\n");
            assert_tree (s);
        }
        		
        //printf ("what is your problem a?\n");
        *new_node->value = strtol (&buf[(*i)++], NULL, 10);
        
        //printf ("it is val of node : %d\n", *new_node->value);
        //printf ("i %d\n", *i);
        if ('(' == buf[*i])
        {
			//printf ("i found right node %d\n", *i);
            new_node->right = node_read (s, i, buf, new_node);
            //printf ("oooooookey\n");
        }
    }
    
    if (')' == buf[*i])
    {
		(*i)++;
		return new_node;
	}
	
    //printf ("uuu %d\n", *i);
	
	assert_tree (s);
    return NULL;
}
//----------------------------------------------------------------------------------------------------------------------
int tree_ctor (tree_t *s)
{
    assert (s);

    s->root = NULL;
    s->count = 0;

    return TREE_ERROR;
}

node * node_ctor ()
{
	
    node * new_node = (node *) calloc (1, sizeof(*new_node));
    if (new_node == NULL)
    {
        TREE_ERROR = ALLOC_ERROR;
        return NULL;
    }
    //printf ("i created new node\n");
    
    new_node->value = (tree_val *) calloc (MAX_LINE_SIZE, sizeof (*new_node->value));
    if (new_node->value == NULL)
    {
        TREE_ERROR = ALLOC_ERROR;
        return NULL;
    }
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;

    return new_node;

}

int tree_dtor (tree_t *s)
{
    assert (s);
    node *tmp_node = s->root;
    tree_clean(s, tmp_node);

    s->root = NULL;
    s->count = COUNT_POISON;

    return TREE_ERROR;
}

int node_dtor (node *s)
{
    assert(s);
    s->value = NULL;
    free (s->value);

    s = NULL;

    return TREE_ERROR;
}

int tree_clean (tree_t *s, node *tmp_node)
{
    assert (s) ;

    for (int i = 0; i < s->count; i++)
    {
        node *tmp_nodeR = tmp_node->right;
        node *tmp_nodeL = tmp_node->left;
        if (tmp_nodeR != NULL)		node_dtor (tmp_nodeR);
        if (tmp_nodeL != NULL)		node_dtor (tmp_nodeL);	
    }
    return TREE_ERROR;
}

//----------------------------------------------------------------------
int is_tree_ok (tree_t *s)
{
    assert(s);
    if (TREE_ERROR != NO_ERROR)
    {
		printf ("1");
        return TREE_ERROR;
    }
    if (s->count < 0) 
    {
        printf("ERROR COUNT\n");
        return TREE_ERROR = COUNT_ERROR;
    }

    /*if (s->count != 0 && s->root == NULL )
    {
		printf("NULL_POINTER_ERROR\n");
        return TREE_ERROR = NULL_POINTER_ERROR;
        
    }*/
/*
    if (s->count != 0)
    {
		printf ("1");
        node *tmp_elem = s->root->next;

        for (int i = 1; i < s->count; i++)
        {
            if (tmp_elem->prev != NULL && tmp_elem->next != NULL)
            {
                if (tmp_elem->prev->next != tmp_elem->next->prev) return TREE_ERROR = WRONG_POINTER_ERROR;
                else tmp_elem = tmp_elem->next;
            }
            else if (tmp_elem->prev == NULL && tmp_elem != s->root)
            {
                return TREE_ERROR = WRONG_POINTER_ERROR;
            }
        }
    }*/
    return TREE_ERROR;
}

void tree_print (tree_t *s)
{
    printf ("\n # list %p root %8p count %3d\n", s, s->root, s->count);
    node * tmp_elem = s->root;
    printf ("meow\n");
    node_print (tmp_elem);
}

void node_print (node *tmp_elem)
{
	printf ("meow2\n");
	if (tmp_elem != NULL)
	{
		printf ("%5d : cur : %15p \n\t parent: %15p\n\t left : %15p\n\t right : %15p\n", *tmp_elem->value, tmp_elem, tmp_elem->parent, tmp_elem->left, tmp_elem->right);
		node_print (tmp_elem->right);
		node_print (tmp_elem->left);
	}
	return;
}
/*
int tree_dot_print (tree_t *s)
{
    FILE *file_dot = fopen ("file_dot.dot", "w");
    if (file_dot == NULL)
    {
        printf ("Opening file failure\n");
        TREE_ERROR = F_OPEN_ERROR;

        return TREE_ERROR;
    }

    fprintf (file_dot, "digraph G{\n");
    fprintf (file_dot,  "node[color=\"green\",fontsize=10, fillcolor=\"lightgrey\"];\n");
    fprintf (file_dot,  "edge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n");
    fprintf (file_dot,  "rankdir=HR;\n");
    node *tmp_elem = s->root;
    for (int i = 0; i < s->count; i++)
    {
        if (tmp_elem->next != NULL)
        {

            fprintf (file_dot, "cur_elem [shape = record,label = \"  <value> value:\\n%d | <next> next:\\n%p | <prev> prev:\\n%p | <left> left:\\n%p| <right> right:\\n%p\"];\n", *tmp_elem->value, tmp_elem->next, tmp_elem->prev, tmp_elem->left, tmp_elem->right);
            fprintf (file_dot, "next_elem [shape = record,label = \"  <value> value:\\n%d | <next> next:\\n%p | <prev> prev:\\n%p | <left> left:\\n%p| <right> right:\\n%p\"];\n", *tmp_elem->next->value, tmp_elem->next->next, tmp_elem->next->prev, tmp_elem->next->left, tmp_elem->next->right);
            fprintf (file_dot, "cur_elem:<next> -> next_elem;\n");

            tmp_elem = tmp_elem->next;
        }
    }


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

    return TREE_ERROR;
}
*/
