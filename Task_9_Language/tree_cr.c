#include "head.h"

#define skipny while (*s == ' ' || *s == '\n' || *s == '\t') s++


char *s = NULL;





/*
typedef struct func
{
    char *name;
    int count; //кол-во параметров
} func;

int ff = 0;*/
/*
int find_func(char *name, func *f_mem);
int num_func(char *name, func *f_mem);
*/


node *getNG (char *buf, tree_t *equ, func * f_mem) //main G::= [F]+'end'
{
    s = buf;
    node *node_ng = node_ctor();
    equ->root = node_ng;
    node_ng->type = 'b';
    node_ng->value = 'b';
    skipny;
    node *node_op = getF(equ, f_mem);
    node *node_tmp = NULL;
    node_tmp = node_op;
    node_op->parent = node_ng;
    skipny;
    while (strncmp(s, "x_x", 3))
    {

        node *node_op2 = getF (equ, f_mem);
        node_new_add (equ, node_tmp, node_op2, 'R');
        node_tmp = node_op2;
        skipny;
    }
    node *ex = node_create('X', 'X', NULL, NULL);
    node_tmp->right = ex;
    node_ng->right = node_op;


    return node_ng;
}

node *getF (tree_t *equ, func * f_mem) //func F ::= }
{
    assert (!strncmp(s, "T-T", 3));
    s +=3;
    skipny;
    char name[10] = "0";
    int i = 0;
    while (*s != ' ')
    {
        name[i++] = *s;
        s++;
    }
    f_mem[ff].name = strdup(name);

    skipny;
    assert (*s == '(');
    s++;
    node *node_f0 = NULL;
    node *node_e = NULL;

    node *node_tmp = NULL;

    if ('a' <= *s && *s <= 'z')
    {
        node_e = getZ(equ);
        node_e->type = 'V';
        f_mem[ff].count_params++;
        f_mem[ff].all_stuff_count++;
        skipny;
    }
    assert(*s == ',' || *s == ')' );
    if (*s == ',') s++;

    node_f0 = node_e;

    while (*s != ')')
    {
        skipny;
        if ('a' <= *s && *s <= 'z')
        {
            f_mem[ff].count_params++;
            f_mem[ff].all_stuff_count++;
            node_tmp = getZ(equ);
            node_tmp->type = 'V';
            node_new_add(equ, node_e, node_tmp, 'R');
            node_e = node_tmp;
            assert(*s == ',' || *s == ')' );
        }
        else printf("compilation error\n");
        if (*s == ',') s++;
        skipny;
    }
    s++;
    skipny;

    node *node_m = getM(equ, f_mem);
    node *node_d = node_create('d', f_mem[ff].count_params, node_f0, node_m);
    assert (*s == ';');
    s++;

    count_func ++;

    return node_create('F', ff++, node_d, NULL);
}


node *getM (tree_t *equ, func *f_mem) //operators M ::= '{'[Op]*'}'
{
    assert (*s == '{');
    s++;
    skipny;

    node *node_m = getOp(equ, f_mem);
    //node *node_tmp = NULL;
    node *node_op = node_m;

    skipny;

    while (*s != '}')
    {
        node *node_op2 = getOp (equ, f_mem);
        node_new_add(equ, node_op, node_op2, 'R');
        node_op = node_op2;
        skipny;
    }
    s++;

    return node_m;
}

node *getOp (tree_t *equ, func *f_mem) //operators Op ::= (A|If|W|F|O)';'
{
    node *node_op = node_ctor();
    node_op->type = 'P';
    node_op->value = 'P';

    skipny;

    switch (*s)
    {
        case '^': //^_^ if
            s += 3;
            node_op->left = getIf(equ, f_mem);
            break;
        case '$': //$_$ while
            s += 3;
            node_op->left = getW(equ, f_mem);
            break;
        case '0': //0_0 write
            printf ("1");
            s += 3;
            node_op->left = getO(equ, f_mem);
            break;
        case '#': //#_#
            s += 3;
            node_op->left = getFu(equ, f_mem);
            break;
        case '*': //*.* prisv
            printf ("1");
            s += 3;
            node_op->left = getA(equ, f_mem);
            break;
        case '%': //%-%
            s+=3;
            node_op->left = getR(equ, f_mem);

            break;
        default:
            printf ("something wrong\n");
    }

    assert (*s == ';');
    s++;
    skipny;
    return node_op;
}

node *getR (tree_t *equ, func *f_mem)
{
    node *node_z = getZ(equ);

    //varies[var_pos] = (vari *) calloc (1, sizeof(vari));
    //varies[var_pos]->name = (char) node_z->value;
    //varies[var_pos]->pos = var_pos;
    //var_pos++;
    f_mem[ff].all_stuff_count++;
    return node_create ('o', 'r', node_z, NULL);
}

node *getFu (tree_t *equ, func * f_mem) //func Fu ::= 'F'('x, y, ...)
{
    int i = 0;
    char name[10] = "0";
    while (*s != ' ')
    {
        name[i++] = *s;
        s++;
    }
    int num = num_func(name, f_mem);

    int count = find_fcount_params(name, f_mem);

    node *node_fu = node_create('f', num, NULL, NULL);

    if (count == 0) return node_fu;

    skipny;
    assert(*s == '(');

    s++;

    node *node_op = getZ(equ);
    s--;
    node *node_op1 = getZ(equ);
    node_op->type = 'p';
    node_op1->type = 'u';
    node_fu->left = node_op;
    node_fu->right = node_op1;
    node_op->parent = node_fu;
    node_op1->parent = node_fu;

    assert(*s == ',' || *s == ')');
    if (*s == ',') s++;
    skipny;

    while (*s != ')')
    {
        skipny;
        node *node_op2 = getZ (equ);
        s--;
        node *node_op3 = getZ (equ);
        node_op2->type = 'p';
        node_op3->type = 'u';
        node_new_add(equ, node_op, node_op2, 'R');
        node_new_add(equ, node_op1, node_op3, 'R');
        node_op = node_op2;
        node_op1 = node_op3;
        assert(*s == ',' || *s == ')');
        if (*s == ',') s++;
        skipny;
    }

    assert(*s == ')');
    s++;

    return node_fu;
}

node *getW (tree_t *equ, func *f_mem) //for W ::= '$_$' (C) op
{
    skipny;

    assert (*s == '(');
    s++;
    node *node_c = getC(equ, f_mem);
    assert (*s == ')');
    s++;
    node *node_m = NULL;
    skipny;
    if (*s == '{')	node_m = getM (equ, f_mem);
    else node_m = getOp (equ, f_mem);

    return node_create ('o', 'w', node_c, node_m);
}

node *getIf (tree_t *equ, func *f_mem) //if If::= ^_^ (C) Op; слева условие, справа операторы
{
    skipny;
    assert (*s == '(');
    s++;
    node *node_c = getC(equ, f_mem);
    assert (*s == ')');
    s++;
    node *node_m = NULL;
    skipny;
    if (*s == '{')	node_m = getM (equ, f_mem);
    else node_m = getOp (equ, f_mem);

    return node_create ('o', 'i', node_c, node_m);
}

/*
node *getG (char *buf, tree_t *equ)
{
	s = buf;
	equ->root = getE (equ);
	//printf ("\nGval : %lg\n", equ->root->value);
	assert (*s == '\n');
	s++;
	assert (*s == '\0');
	return equ->root;
}
*/
node *getA (tree_t *equ, func *f_mem) // A ::= *.* Z'='E *в левой ветке имя, в правой - значение*
{
    skipny;
    node *node_z = getZ (equ);
    skipny;
    assert (*s == '=');
    s++;
    skipny;
    node *node_e = NULL;
    if (!strncmp(s, "U.U", 3)) node_e = getI (equ, f_mem);
    else node_e = getE (equ, f_mem);

    return node_create ('o', '=', node_z, node_e);

}

node *getI (tree_t *equ, func *f_mem) //input I ::= U.U
{
    s += 3;
    return node_create ('o', 'I', NULL, NULL);
}

node *getO (tree_t *equ, func *f_mem) //output O ::=
{
    skipny;
    assert(*s == '(');
    s++;
    node *node_e = getE(equ, f_mem);
    assert(*s == ')');
    s++;

    return node_create('o', 'O', node_e, NULL);
}

node *getC (tree_t *equ, func *f_mem) //compare C ::= E ['==', '<', '>'] E
{
    skipny;
    node *node_e1 = getE(equ, f_mem);
    skipny;
    char sign = 0;
    if (!strncmp(s, "O_O", 3)) sign = 'e';
    else if (!strncmp(s, "o_O", 3)) sign = '<';
    else if (!strncmp(s, "O_o", 3)) sign = '>';
    else if (!strncmp(s, "o_o", 3)) sign = '!';
    s += 3;
    skipny;
    node *node_e2 = getE(equ, f_mem);

    return node_create ('o', sign, node_e1, node_e2);
}

node *getE (tree_t *equ, func *f_mem)
{
    skipny;
    node *node_t = getT (equ, f_mem);
    node *node_e = NULL;
    skipny;
    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        node_e = node_ctor ();
        node_e->type = 'o';
        node_e->value = op;
        skipny;
        node *node_t2 = getT(equ, f_mem);
        node_new_add (equ, node_e, node_t, 'L');
        node_new_add (equ, node_e, node_t2, 'R');
        node_t = node_e;
    }
    //printf ("return from t to e\n");
    return node_t;
}

node *getT (tree_t *equ, func *f_mem)
{
    skipny;
    node *node_p = getP (equ, f_mem);
    node *node_t = NULL;
    skipny;
    while (*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        node_t = node_ctor ();
        node_t->type = 'o';
        node_t->value = op;
        skipny;
        node *node_p2 = getP(equ, f_mem);
        node_new_add (equ, node_t, node_p, 'L');
        node_new_add (equ, node_t, node_p2, 'R');
        node_p = node_t;
    }
    //printf ("return from p to t\n");
    return node_p;

}

node *getP (tree_t *equ, func *f_mem)
{
    skipny;
    if (*s == '(')
    {
        skipny;
        s++;
        node *node_p = getE (equ, f_mem);
        skipny;
        assert (*s == ')');
        s++;
        //printf ("in P: %s", s);
        return node_p;
    }
    else if (!(strncasecmp ("sqrt", s, 4))) /*в правой ветке выражение*/
    {
        s += 4;
        skipny;
        node *node_p = getP(equ, f_mem);
        return node_create ('o', 'S', NULL, node_p); //sqrt - S
    }
    else if (!(strncasecmp ("sin", s, 3)) || !(strncasecmp ("cos", s, 3))) /*в правой ветке выражение*/
    {
        double op = *s;
        s += 3;
        skipny;
        node *node_p = getP(equ, f_mem);
        return node_create ('o', op, NULL, node_p);
    }
    else if (!(strncasecmp ("ln", s, 2))) /*в правой ветке выражение*/
    {
        double op = *s;
        s += 2;
        skipny;
        node *node_p = getP(equ, f_mem);
        return node_create ('o', op, NULL, node_p);
    }
    else if (!(strncasecmp ("exp", s, 3))) /*в правой ветке выражение*/
    {
        double op = *s;
        s += 3;
        skipny;
        node *node_p = getP(equ, f_mem);
        return node_create ('o', op, NULL, node_p);
    }
    else if ('a' <= *s && *s <= 'z')
    {
        node *node_z = getZ (equ);
        skipny;
        if (*s == '^') /*в левой ветке выражение, в правой степень*/
        {
            double op = *s;
            s++;
            skipny;
            node *node_p = getP(equ, f_mem);
            return node_create ('o', op, node_z, node_p);
        }
        return node_z;
    }
    else
    {
        skipny;
        node *node_p = getN (equ);
        //printf ("1\n");
        return node_p;
    }
}

node *getN (tree_t *equ)
{
    skipny;
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
    skipny;
    node *node_z = node_ctor ();
    node_z->value = *s;
    node_z->type = 'v';
    s++;
    //printf ("\nZval : %lg\n", node_z->value);

    return node_z;
}

int f_size (FILE* file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}

int find_fcount_params(char *name, func *f_mem)
{
    int i = 0;
    while (strcmp(name, f_mem[i].name))
    {
        i++;
    }
    return f_mem[i].count_params;
}

int num_func(char *name, func *f_mem)
{
    int i = 0;
    while (strcmp(name, f_mem[i].name))
    {
        i++;
    }
    return i;
}

