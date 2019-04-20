#include "head.h"

const int MAX_FILE_SIZE = 100;

const int MAX_LINE_SIZE = 30;

int TREE_ERROR = NO_ERROR;

const int COUNT_POISON = -1;



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

    new_node->value = 0;
    new_node->type = 0;
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
    s->value = VALUE_POISON;

    s->right = NULL;
    s->left = NULL;

    free(s);

    return TREE_ERROR;
}

int tree_clean (tree_t *s, node *tmp_node)
{
    assert (s) ;
    if (tmp_node->right != NULL) tree_clean(s, tmp_node->right);
    if (tmp_node->left != NULL) tree_clean(s, tmp_node->left);
    node_dtor(tmp_node);
    /*
    for (int i = 0; i < s->count; i++)
    {
        node *tmp_nodeR = tmp_node->right;
        node *tmp_nodeL = tmp_node->left;
        if (tmp_nodeR != NULL)		node_dtor (tmp_nodeR);
        if (tmp_nodeL != NULL)		node_dtor (tmp_nodeL);
    }*/
    return TREE_ERROR;
}
node *node_new_add (tree_t *s, node *prev, node *elem, char pos)
{
    if (pos == 'L')
    {

        elem->parent = prev;
        prev->left = elem;
        s->count++;
        assert_tree(s);
        //printf("new1\n");
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
node *node_add (tree_t *s, node *prev, tree_val val, char pos)
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
        printf ("%5lg : cur : %15p \n\t parent: %15p\n\t left : %15p\n\t right : %15p\n", tmp_elem->value, tmp_elem, tmp_elem->parent, tmp_elem->left, tmp_elem->right);
        node_print (tmp_elem->right);
        node_print (tmp_elem->left);
    }
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
        new_node->value = strtol (&buf[(*i)++], NULL, 10);

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
    fprintf (file_out, "%lg", n_node->value);
    if (n_node->right != NULL)
    {
        node_write (n_node->right, file_out);
    }
    fprintf (file_out, ")");

    return;
}
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
    return TREE_ERROR;
}

void tree_dot_print (tree_t *s)
{
    FILE *file_dot = fopen("lang_dot.dot", "w");
    if (file_dot == NULL)
    {
        printf("Do not open file\n");
        return;
    }
    fprintf(file_dot, "digraph\n{\n");
    fprintf(file_dot, "node[shape = \"septagon\", color=\"darkgreen\",fontsize=10, style=\"filled\", fillcolor = \"#9CC98D\"];\nedge[color=\"darkgreen\",fontsize=12];\n");
    create_tree (s->root, file_dot);
    fprintf(file_dot, "}");
    fclose(file_dot);
    system("dot -Tjpg -o tree.jpg lang_dot.dot");
    system("xdg-open tree.jpg");
    return;
}
/*
void create_tree (node *s, FILE *f)
{
	if (s->left != NULL)
	{
		fprintf (f, "\t%f[shape = \"egg\", color=\"green\", style=\"filled\", fillcolor = \"white\"];\n\t%f->%f;\n", s->value, s->value, s->left->value);
		create_tree (s->left, f);
	}
	if (s->right != NULL)
	{
		fprintf (f, "\t%f[shape = \"egg\", color=\"green\", style=\"filled\", fillcolor = \"white\"];\n\t%f->%f;\n", s->value, s->value, s->right->value);
		create_tree (s->right, f);
	}
	return;
}
*/
int tmp = 0;
void create_tree (node* pos, FILE* dot_out)
{
    fprintf(dot_out, "\t\they%d", tmp);
    print_type (pos, dot_out);
    fprintf(dot_out, ";\n");

    int i = tmp;

    if (pos->left != NULL) {
        fprintf(dot_out, "\t\they%d", i);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d[label=\"left\",style=\"dashed\",arrowhead=\"dot\"];\n", ++tmp);

        create_tree(pos->left, dot_out);
    }

    if (pos->right != NULL) {
        fprintf(dot_out, "\t\they%d", i);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d[label=\"right\",style=\"dashed\",arrowhead=\"dot\"];\n", ++tmp);

        create_tree(pos->right, dot_out);
    }
}

void print_type(node* pos, FILE* dot_out)
{
    switch (pos->type)
    {
        case type_OP:     fprintf(dot_out, "[label = \"%c\", shape = \"circle\", "
                                           "color=\"white\", style=\"filled\", "
                                           "fillcolor = \"#C390D4\"]", (int) pos->value);
            break;

        case type_CONST:   fprintf(dot_out, "[label = \"%f\", shape = \"septagon\", "
                                            "color=\"white\", style=\"filled\", "
                                            "fillcolor = \"#A1D490\"]", pos->value);
            break;

        case type_VAR:    fprintf(dot_out, "[label = \"%c\", shape = \"septagon\", "
                                           "color=\"white\", style=\"filled\", "
                                           "fillcolor = \"#90D4C1\"]", (int) pos->value);
            break;
        case type_v:    fprintf(dot_out, "[label = \"%c\", shape = \"septagon\", "
                                           "color=\"white\", style=\"filled\", "
                                           "fillcolor = \"#F5C32F\"]", (int) pos->value);
            break;

        case type_NG:    fprintf(dot_out, "[label = \"%c\", shape = \"octagon\", "
                                          "color=\"white\", style=\"filled\", "
                                          "fillcolor = \"#90C3D4\"]", (int) pos->value);
            break;

        case type_P:    fprintf(dot_out, "[label = \"%c\", shape = \"octagon\", "
                                         "color=\"white\", style=\"filled\", "
                                         "fillcolor = \"#90C3D4\"]", (int) pos->value);
            break;
        case type_Ex:    fprintf(dot_out, "[label = \"%c\", shape = \"octagon\", "
                                          "color=\"white\", style=\"filled\", "
                                          "fillcolor = \"#90C3D4\"]", (int) pos->value);
            break;
        case type_F:    fprintf(dot_out, "[label = \"%d\", shape = \"septagon\", "
                                          "color=\"white\", style=\"filled\", "
                                          "fillcolor = \"#9F90D4\"]", (int) pos->value);
            break;
        case type_f:    fprintf(dot_out, "[label = \"%d\", shape = \"septagon\", "
                                         "color=\"white\", style=\"filled\", "
                                         "fillcolor = \"#2FF546\"]", (int) pos->value);
            break;
        case type_d:    fprintf(dot_out, "[label = \"%d\", shape = \"septagon\", "
                                         "color=\"white\", style=\"filled\", "
                                         "fillcolor = \"#D4A190\"]", (int) pos->value);
            break;
        case type_p:    fprintf(dot_out, "[label = \"%d\", shape = \"septagon\", "
                                         "color=\"white\", style=\"filled\", "
                                         "fillcolor = \"#D4A190\"]", (int) pos->value);
            break;
        case type_u:    fprintf(dot_out, "[label = \"%d\", shape = \"septagon\", "
                                         "color=\"white\", style=\"filled\", "
                                         "fillcolor = \"#D4A190\"]", (int) pos->value);
            break;
        default:          fprintf(dot_out, "\"error\"");
            break;
    }
}
void tree_koncole(node *s) {
    assert (s);
    printf("%c", '(');
    if (s->left)
    {
        tree_koncole (s->left);
    }

    if (s->type == 'o' || s->type == 'v')
        printf("%c", (int)s->value);
    else
    {
        printf("%f", s->value);
    }


    if (s->right)
    {
        tree_koncole (s->right);
    }
    printf("%c", ')');

}

node* tree_copy (node* cur)
{
    node* new1 = node_ctor ();
    new1->value = cur->value;
    new1->type =  cur->type;
    if (cur->left)
    {
        node* l_new = tree_copy (cur->left);
        new1->left = l_new;
        l_new->parent = new1;
    }

    if (cur->right)
    {
        node* r_new = tree_copy (cur->right);
        new1->right = r_new;
        r_new->parent = new1;
    }
    return new1;
}

node *node_create (char type, double val, node *lefty, node *righty)
{
    node *new1 = node_ctor ();
    new1->type = type;
    new1->value = val;
    new1->left = lefty;
    new1->right = righty;
    if (lefty) lefty->parent = new1;
    if (righty) righty->parent = new1;

    return new1;
}

node * up_node (tree_t *equ, node *cur, char pos)
{
    assert (equ);
    assert (cur);

    node * up = NULL;
    if (pos == 'L')
    {
        up = cur->left;
        //free(cur->right);
        cur->right = NULL;
    }
    else if (pos == 'R')
    {
        up = cur->right;
        //free(cur->left);
        cur->left = NULL;
    }

    if (cur != equ->root)
    {
        up->parent = cur->parent;
        if (cur->parent->left == cur) cur->parent->left = up;
        else if (cur->parent->right == cur) cur->parent->right = up;
        (equ->count)--;
        //free(cur);
        cur = NULL;

        return up;
    }
    else
    {
        equ->root = up;
        up->parent = NULL;
        (equ->count)--;
        //free(cur);
        cur = NULL;

        return up;
    }
    return NULL;
}

node * up2_node (tree_t *equ, node *cur, char pos, double val)
{
    assert (equ);
    assert (cur);
    node * up = NULL;

    if (pos == 'L')
    {
        up = cur->left;
        cur->right = NULL;
    }
    else if (pos == 'R')
    {
        up = cur->right;
        cur->left = NULL;
    }
    if (up->left)
    {
        up->right->value += val;
    }
    if (cur != equ->root)
    {
        up->parent = cur->parent;
        if (cur->parent->left == cur) cur->parent->left = up;
        else if (cur->parent->right == cur) cur->parent->right = up;

        (equ->count)--;
        cur = NULL;
    }
    else
    {
        equ->root = up;
        up->parent = NULL;
        (equ->count)--;
        cur = NULL;
    }
    return NULL;
}
void short_node (tree_t *equ, node *op1, char pos1, char pos2) //для преобразования + - в середине дерева
{
    node *op2 = NULL;
    node *ch1 = NULL;
    node *ch2 = NULL;

    if (pos1 == 'L')
    {
        op2 = op1->left;
        ch1 = op1->right;
    }
    else if (pos1 == 'R')
    {
        op2 = op1->right;
        ch1 = op1->left;
    }

    if (pos2 == 'L')
    {
        ch2 = op2->right;
    }
    else if (pos2 == 'R')
    {
        ch2 = op2->left;
    }

    int val = op1->value;
    op1->value = op2->value;
    op2->value = val;
    printf ("val1: %d\n", (int) op1->value);
    printf ("val2: %d\n", (int) op2->value);

    if (pos1 == 'L')
    {
        if (pos2 == 'L')
        {
            op1->right = ch2;
            ch2->parent = op1;

            op2->right = ch1;
            ch1->parent = op2;
        }
        else
        {
            op1->right = ch2;
            ch2->parent = op1;

            op2->left = ch1;
            ch1->parent = op2;
        }
    }
    else
    {
        if (pos2 == 'L')
        {
            op1->left = ch2;
            ch2->parent = op1;

            op2->right = ch1;
            ch1->parent = op2;
        }
        else
        {
            op1->left = ch2;
            ch2->parent = op1;

            op2->left = ch1;
            ch1->parent = op2;
        }
    }
}

node * equ_node (tree_t *equ, node *cur, char pos)
{
    cur->type = type_CONST;
    (equ->count) -= 2;
    if (pos == '+') cur->value = cur->left->value + cur->right->value;
    if (pos == '-') cur->value = cur->left->value - cur->right->value;
    if (pos == '*') cur->value = cur->left->value * cur->right->value;
    if (pos == '/') cur->value = cur->left->value / cur->right->value;
    //if (pos == '^') cur->value = pow(cur->left->value,cur->right->value);
    free(cur->left);
    free(cur->right);
    cur->left = NULL;
    cur->right = NULL;
    return cur;
}
node *zero_node (node * cur)
{
    cur->value = 0;
    cur->type = 'c';
    cur->left = NULL;
    cur->right = NULL;

    return cur;
}
node *change_node (node * cur)
{
    node * tmp = cur->left;
    cur->left = cur->right;
    cur->right = tmp;

    return cur;
}
