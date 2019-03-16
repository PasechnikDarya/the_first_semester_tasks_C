#include <stdio.h>
#include "tree_creater.c"

#define l(a) (a)->left
#define r(a) (a)->right

#define dif(a) differentiator(a)
#define c(a) tree_copy(a)

#define CONST(n) node_create (type_CONST, 0, NULL, NULL)
#define VAR(n) node_create (type_CONST, 1, l(n), r(n))
#define PLUS(n1, n2) node_create (type_OP, '+', n1, n2)
#define SUB(n1, n2) node_create (type_OP, '-', n1, n2)
#define MUL(n1, n2) node_create (type_OP, '*', n1, n2)
#define DIV(n1, n2) node_create (type_OP, '/', n1, n2)
#define SIN(n) node_create (type_OP, 's', NULL, n)
#define COS(n) node_create (type_OP, 'c', NULL, n)
#define POW(n1, n2) node_create (type_OP, '^', n1, n2)
#define LN(n) node_create (type_OP, 'l', NULL, n)
#define EXP(n) node_create (type_OP, 'e', NULL, n)
#define NUM(a) node_create (type_CONST, (a), NULL, NULL)

#define is_mul(n) n->value == '*' 
#define is_div(n) n->value == '/' 
#define is_add(n) n->value == '+' 
#define is_sub(n) n->value == '-' 
#define is_pow(n) n->value == '^'
#define is_zero(n) n->value == 0
#define is_one(n) n->value == 1
#define is_const(n) n->type == type_CONST
#define is_op(n) n->type == type_OP
#define is_var(n) n->type == type_VAR

#define op_eq fprintf (f, "(")
#define cl_eq fprintf (f, ")")
#define op_feq fprintf (f, "{")
#define cl_feq fprintf (f, "}")
#define frac fprintf (f, "\\frac{")
 

node *differentiator (node * cur_node);
node *tree_simple (tree_t *equ, node *tmp);
node *node_simple (tree_t *equ, node *cur);
void tree_latex_print (node *equ, FILE * file);
void latex_print (tree_t * equ, char * file_name);

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
	/*
	tree_t *dif_equ = (tree_t *) calloc (1, sizeof (*dif_equ));
	tree_ctor (dif_equ);
	
	dif_equ->root = differentiator (equ->root);
	//tree_dot_print (dif_equ);
	
	tree_simple (dif_equ, dif_equ->root);
	tree_simple (dif_equ, dif_equ->root);
	
	printf("\n");
	tree_koncole (dif_equ->root);
	tree_dot_print (dif_equ);*/

	//tree_write (dif_equ, "file_out.txt");
	
	latex_print (equ, "eq.tex");
	
	//tree_dtor (dif_equ);
	tree_dtor (equ);
	return 0;
}

node *differentiator (node *cur)
{
	node *new = NULL;
	switch (cur->type)
	{
		case type_CONST:
        {
            new = CONST(cur);
            break;
        }

        case type_VAR:
        {
            new = VAR(cur);
            break;
        }

        case type_OP:
        {
            switch ((int) cur->value)
            {
                case '+':
                {
					new = PLUS(dif(l(cur)), dif(r(cur)));
                    break;
                }

                case '-':
                {
                    new = SUB(dif(l(cur)), dif(r(cur)));
                    break;
                }
                case '*':
                {
					if (is_const (r(cur)) && is_const (l(cur))) new = MUL(c(r(cur)), c(l(cur)));
					else new = PLUS(MUL(dif(l(cur)), c(r(cur))), MUL(dif(r(cur)), c(l(cur))));
                    break;
                }
				case 's':
                {
					new = MUL(COS(r(cur)), dif(r(cur)));
                    break;
                }

                case 'c':
                {
					new = MUL(NUM(-1), MUL(SIN(r(cur)), dif(r(cur))));
                    break;
                }
                case '^':
                {
					new = MUL(MUL(c(r(cur)), POW(c(l(cur)), NUM(r(cur)->value - 1))), dif(l(cur)));
					break;
				}
				case '/':
				{
					new = DIV(SUB(MUL(dif(l(cur)), c(r(cur))), MUL(dif(r(cur)), c(l(cur)))), POW(c(r(cur)), NUM(2)));
					break;
				}
				case 'l':
                {
					new = MUL(DIV(NUM(1), c(r(cur))), dif(r(cur)));
					break;
				}
				case 'e':
                {
					new = MUL(cur, dif(r(cur)));
					break;
				}
                default:
                {
                    printf ("Unknown operation\n");
                    printf ("operation is %c", (int)cur->value);
                }

            }
            break;
        }
        
        default:
        {
            printf("Unknown type\n");
            printf("type is %d", cur->type);
        }
	}
	return new;
}
node *tree_simple (tree_t *equ, node *tmp)
{
	assert(tmp);
	
	if (l(tmp)) tree_simple(equ, l(tmp));
	if (r(tmp)) tree_simple(equ, r(tmp));
	node_simple (equ, tmp);
	if (l(tmp)) tree_simple(equ, l(tmp));
	if (r(tmp)) tree_simple(equ, r(tmp));
	
	return tmp;
}
node *node_simple (tree_t *equ, node *cur)
{
	switch ((int) cur->value)
	{
		case '+':
			if (is_zero (l(cur)) && is_zero (r(cur)))
			{
				zero_node (cur);
			}
			else if (is_const (l(cur)) && is_const (r(cur)))
			{
				equ_node (equ, cur, '+');
			}				
			else if (is_zero (l(cur)))
			{
				up_node (equ, cur, 'R');
			}
			else if (is_zero (r(cur)))
			{
				up_node (equ, cur, 'L');
			}
			else if ((is_add (l(cur)) || is_sub (l(cur))) && is_const (r(cur)))
			{
				if (is_const (l(l(cur)))) short_node (equ, cur, 'L', 'L');
				else if (is_const (r(l(cur)))) short_node (equ, cur, 'L', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_add (r(cur)) || is_sub (r(cur))) && is_const (l(cur)))
			{
				if (is_const (l(r(cur)))) short_node (equ, cur, 'R', 'L');
				else if (is_const (r(r(cur)))) short_node (equ, cur, 'R', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_add (l(cur)) || is_sub (l(cur))) && is_var (r(cur)))
			{
				if (is_var (l(l(cur)))) short_node (equ, cur, 'L', 'L');
				else if (is_var (r(l(cur)))) short_node (equ, cur, 'L', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_add (r(cur)) || is_sub (r(cur))) && is_var (l(cur)))
			{
				if (is_var (l(r(cur)))) short_node (equ, cur, 'R', 'L');
				else if (is_var (r(r(cur)))) short_node (equ, cur, 'R', 'R');
				//tree_simple (equ, cur);
			}
			else if (is_var (l(cur)) && is_var (r(cur)))
			{
				cur->value = '*';
				cur->type = 'o';
				cur->left->value = 2;
				cur->left->type = 'c';
			}
			break;
		case '-':
			if (is_zero (l(cur)) && is_zero (r(cur)))
			{
				zero_node (cur);
			}
			else if (is_const (l(cur)) && is_const (r(cur)))
			{
				equ_node (equ, cur, '-');
			}
			else if (is_zero (l(cur)))
			{
				cur->value = '*';
				l(cur)->value = -1;
				node_simple (equ, cur);
			}
			else if (is_zero (r(cur)))
			{
				up_node (equ, cur, 'L');
			}
			else if ((is_add (l(cur)) || is_sub (l(cur))) && is_const (r(cur)))
			{
				if (is_const (l(l(cur)))) short_node (equ, cur, 'L', 'L');
				else if (is_const (r(l(cur)))) short_node (equ, cur, 'L', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_add (r(cur)) || is_sub (r(cur))) && is_const (l(cur)))
			{
				if (is_const (l(r(cur)))) short_node (equ, cur, 'R', 'L');
				else if (is_const (r(r(cur)))) short_node (equ, cur, 'R', 'R');
				//tree_simple (equ, cur);
			}
			break;
		case '*':
			if (is_zero (l(cur)) || is_zero (r(cur)))
			{
				zero_node (cur);
			}
			else if (is_const (l(cur)) && is_const (r(cur)))
			{
				equ_node (equ, cur, '*');
			}
			else if (is_one (l(cur)))
			{
				up_node (equ, cur, 'R');
			}
			else if (is_one (r(cur)))
			{
				up_node (equ, cur, 'L');
			}
			else if (is_const (r(cur))) change_node (cur);
			else if ((is_div (l(cur)) || is_mul (l(cur))) && is_const (r(cur)))
			{
				if (is_const (l(l(cur)))) short_node (equ, cur, 'L', 'L');
				else if (is_const (r(l(cur)))) short_node (equ, cur, 'L', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_div (r(cur)) || is_mul (r(cur))) && is_const (l(cur)))
			{
				if (is_const (l(r(cur)))) short_node (equ, cur, 'R', 'L');
				else if (is_const (r(r(cur)))) short_node (equ, cur, 'R', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_div (l(cur)) || is_mul (l(cur))) && is_var (r(cur)))
			{
				if (is_var (l(l(cur)))) short_node (equ, cur, 'L', 'L');
				else if (is_var (r(l(cur)))) short_node (equ, cur, 'L', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_div (r(cur)) || is_mul (r(cur))) && is_var (l(cur)))
			{
				if (is_var (l(r(cur)))) short_node (equ, cur, 'R', 'L');
				else if (is_var (r(r(cur)))) short_node (equ, cur, 'R', 'R');
				//tree_simple (equ, cur);
			}
			else if (is_var (r(cur)) && is_pow(l(cur)))
			{
				up2_node (equ, cur, 'L', 1);
			}
			else if (is_var (l(cur)) && is_pow(r(cur)))
			{
				up2_node (equ, cur, 'R', 1);
			}
			else if (is_var (l(cur)) && is_var (r(cur)))
			{
				cur->value = '^';
				cur->type = 'o';
				cur->right->value = 2;
				cur->right->type = 'c';
			}
			else if (is_div (l(cur)))
			{
				if (is_one (l(l(cur))))
				{
					short_node (equ, cur, 'L', 'L');
				}
			}
			break;
		case '/':
			if (is_zero (l(cur)))
			{
				zero_node (cur);
			}
			else if (is_const (l(cur)) && is_const (r(cur)))
			{
				equ_node (equ, cur, '/');
			}
			else if (is_one (r(cur)))
			{
				up_node (equ, cur, 'L');
			}
			else if ((is_div (l(cur)) || is_mul (l(cur))) && is_const (r(cur)))
			{
				printf ("1");
				if (is_const (l(l(cur)))) short_node (equ, cur, 'L', 'L');
				else if (is_const (r(l(cur)))) short_node (equ, cur, 'L', 'R');
				//tree_simple (equ, cur);
			}
			else if ((is_div (r(cur)) || is_mul (r(cur))) && is_const (l(cur)))
			{
				if (is_const (l(r(cur)))) short_node (equ, cur, 'R', 'L');
				else if (is_const (r(r(cur)))) short_node (equ, cur, 'R', 'R');
				//tree_simple (equ, cur);
			}
			break;
		case '^':
			if (is_zero (r(cur)))
			{
				r(cur)->value = 1;
				up_node (equ, cur, 'L');
			}
			else if (is_one (r(cur)))
			{
				up_node (equ, cur, 'L');
			}
			else if (is_const (l(cur)) && is_const (r(cur)))
			{
				equ_node (equ, cur, '^');
			}
			break;
			
	}

		return cur;			
}

void latex_print (tree_t * equ, char * file_name)
{
	FILE * latex_file = fopen (file_name, "wb");
	
	fprintf(latex_file,   "\\documentclass[a4paper, 12pt]{article}\n"
							"\\usepackage[T2A]{fontenc}\n"
							"\\usepackage[utf8]{inputenc}\n"
							"\\usepackage[english,russian]{babel}\n"
							"\\usepackage{graphicx}\n"
							"\\usepackage{float}\n"
							"\\usepackage{wrapfig}\n"
							"\\usepackage{fancybox,fancyhdr}\n"
							"\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n"
							"\\usepackage{subfigure}\n"
							"\n"
							"\n"
							"\\author{Пасечник Даша, группа 816}\n"
							"\\title{Лабораторная работа 1}\n"
							"\\title {Изучение влияния взятия достаточно длинных производных на настроение котиков.}\n"
							"\n"
							"\n"
							"\\date{МФТИ, Декабрь 2018}\n"
							"\n"
							"\\begin{document}\n"
							"\\maketitle\n"
							"\\newpage\n");
			
	fprintf (latex_file, "По статистике, когда кто-то не берет производную, в мире грустит один котик.\n");
	fprintf (latex_file, "\\begin{equation}\n");
	tree_latex_print (equ->root, latex_file);
	fprintf (latex_file, "\\end{equation}\n");
	
	fprintf (latex_file, "\\begin{center}\n"
						 "\\includegraphics[scale =0.5]{kotic1.jpg}\n"
						 "\\end{center}\n");
	
	fprintf (latex_file, "Чем больше невзятых производных, тем больше плачущих котиков..\n");
	fprintf (latex_file, "\\begin{center}\n"
						 "\\includegraphics[scale=0.7]{kotic4.jpg}\n"
						 "\\end{center}\n");
						 
	tree_t *dif_equ = (tree_t *) calloc (1, sizeof (*dif_equ));
	tree_ctor (dif_equ);
	dif_equ->root = differentiator (equ->root);
	fprintf (latex_file, "Дифференцирование неумелыми программистами тоже не вызывает у котиков восторга:\n");

	fprintf (latex_file, "\\begin{center}\n$");
	tree_latex_print (dif_equ->root, latex_file);
	fprintf (latex_file, "$\\end{center}\n");
	
	fprintf (latex_file, "\\begin{center}\n"
						 "\\includegraphics[scale=0.3]{kotic2.jpg}\n"
						 "\\end{center}\n");
	fprintf (latex_file, "Кроме того, Минздрав предупреждает, дифференцирование без упрощений может быть опасно для вашего здоровья:\n");

	fprintf (latex_file, "\\begin{figure}[H]\n"
						 "\\vspace{4ex} \\centering \\subfigure{\n"
						 "\\includegraphics[width=0.45\\linewidth]{poltorashechka.jpg} } \n"
						 "\\hspace{1ex}\n"
						 "\\subfigure{\n"
						 "\\includegraphics[width=0.45\\linewidth]{poltorashka.jpg} }\n"
						 "\\end{figure}\n");
	tree_simple (dif_equ, dif_equ->root);
	//tree_simple (dif_equ, dif_equ->root);
	fprintf (latex_file, "Этому городу нужен новый герой..\n");
	fprintf (latex_file, "\\begin{center}\n"
						 "\\includegraphics[scale=0.5]{kotic5.jpg}\n"
						 "\\end{center}\n");
						 
	fprintf (latex_file, "Совсем другое дело:\n");
	
	fprintf (latex_file, "\\begin{equation}\n");
	tree_latex_print (dif_equ->root, latex_file);
	fprintf (latex_file, "\\end{equation}\n");
	
	fprintf (latex_file, "\\begin{center}\n"
						 "\\includegraphics[scale=0.8]{kotic3.jpg}\n"
						 "\\end{center}\n");
	fprintf (latex_file, "Миссия выполнена, мир снова чист от неупрощенных выражений..\n");
	fprintf (latex_file, "Можно отдохнуть..\n");
	fprintf (latex_file, "\\begin{center}\n"
						 "\\includegraphics[scale=0.5]{kotic6.jpg}\n"
						 "\\end{center}\n");
	
	fprintf (latex_file, "\n"
                 "\\end{document}");
    tree_dot_print (dif_equ);
	
	fclose(latex_file);
	
	tree_dtor (dif_equ);
	
}

void tree_latex_print (node *cur, FILE * f)
{
	if (!cur) return;

	if (is_op (cur))
	{
			switch ((int) cur->value)
			{
				case '*':
					if (is_const (l(cur)) || is_var (l(cur)) || is_mul (l(cur)) || is_div (l(cur)) || is_pow (l(cur)))
						tree_latex_print (l(cur), f);
					else
					{
						op_eq;
						tree_latex_print (l(cur), f);
						cl_eq;
					}
					if (is_const (r(cur)) || is_var(r(cur)) || is_mul (r(cur)) || is_div (r(cur)) || is_pow (r(cur)))
						tree_latex_print (r(cur), f);
					else
					{
						op_eq;
						tree_latex_print (r(cur), f);
						cl_eq;
					}
					return;
					break;
				case '/':
					frac;
					tree_latex_print (l(cur), f);
					cl_feq;
					op_feq;
					tree_latex_print(r(cur), f);
					cl_feq;
					return;
					break;
				case '^':
					if (is_const (l(cur)) || is_var (l(cur))) tree_latex_print (l(cur), f);
					else
					{
						op_eq;
						tree_latex_print (l(cur), f);
						cl_eq;
					}
					fprintf (f, "%c", '^');
					op_feq;
					tree_latex_print (r(cur), f);
					cl_feq;
					return;
					break;
				case '-':
					tree_latex_print (l(cur), f);
					fprintf (f, "%c", '-');
					if (is_add(r(cur)) || is_sub(r(cur))) 
					{
						op_eq;
						tree_latex_print (r(cur), f);
						cl_eq;
					}
					else tree_latex_print (r(cur), f);
					
					return;
					break;
			}
	}
			
	tree_latex_print (l(cur), f);
	
	switch (cur->type)
	{
		case type_VAR:
			fprintf (f, "%c", (int) cur->value);
			break;
		case type_CONST:
			fprintf (f, "%.2g", cur->value);
			break;
		case type_OP:
			switch ((int) cur->value)
			{
				case '+':
					fprintf (f, "%c", '+');
					break;
				case '-':
					fprintf (f, "%c", '-');
					break;
				case 's':
					if (is_const (r(cur))) fprintf(f, "\\sin ");
					else if (is_var (r(cur))) fprintf(f, "\\sin %c", (int) r(cur)->value);
					else 
					{
						fprintf(f, "\\sin");
						op_eq;
						tree_latex_print (r(cur), f);
						cl_eq;
					}
					return;
					break;
				case 'c':
					if (is_const (r(cur))) fprintf(f, "\\cos ");
					else if (is_var (r(cur))) fprintf(f, "\\cos %c", (int) r(cur)->value);
					else 
					{
						fprintf(f, "\\cos");
						op_eq;
						tree_latex_print (r(cur), f);
						cl_eq;
					}
					return;
					break;
				case 'l':
					if (is_const (r(cur))) fprintf(f, "\\ln ");
					else if (is_var (r(cur))) fprintf(f, "\\ln %c", (int) r(cur)->value);
					else 
					{
						fprintf(f, "\\ln");
						op_eq;
						tree_latex_print (r(cur), f);
						cl_eq;
					}
					return;
					break;
				case 'e':
					if (is_const (r(cur))) fprintf(f, "\\exp ");
					else if (is_var (r(cur))) fprintf(f, "\\exp %c", (int) r(cur)->value);
					else 
					{
						fprintf(f, "\\exp");
						op_eq;
						tree_latex_print (r(cur), f);
						cl_eq;
					}
					return;
					break;
			}
			break;
	}
	
	tree_latex_print (r(cur), f);
}
