#include "head.h"

int glav_q = 1;
int targ = 10;
int pawpaw = 0;
int q = 1;

int func_name = 0;

void tree_asmer (char * file, tree_t *equ, func * f_mem)
{
    assert(equ);

    FILE *file_out = fopen(file, "w");
    if (!file_out)
    {
        printf("file didn't open\n");

        return;
    }

    fprintf(file_out, "jmp :%d\n", count_func - 1);

    node_asmer (file_out, equ->root, f_mem);
    fprintf(file_out, "meow\n");

    fclose(file_out);
}

void node_asmer (FILE *file, node *node, func *f_mem)
{
    //node *noddy = ;
    if (!node) return;

    if (node->type == 'b') node_asmer(file, node->right, f_mem);
    else if (node->type == 'X') fprintf(file, "end\n");
    else if (node->type == 'F') // function
    {
        func_name = (int) node->value;

        if (node->value == count_func - 1) // main check
        {
            fprintf(file, ":%d\n", (int) node->value);
            node_asmer(file, node->left->right, f_mem);
            node_asmer(file, node->right, f_mem);
        }
        else
        {
            fprintf(file, ":%d\n", (int) node->value);
            node_asmer(file, node->left, f_mem);

            if (node->right) node_asmer(file, node->right, f_mem);
            else fprintf(file, "end\n");

        }

    }
    else if (node->type == 'd')
    {
        glav_q = q - 1;
        q = 1;

        node_asmer(file, node->left, f_mem);
        f_mem[func_name].point = 1;

        fprintf(file, "pushreg ax\n");
        fprintf(file, "push %d\n", f_mem[func_name].all_stuff_count); //ok ff = 2, выход за границы массива
        fprintf(file, "add\n");
        fprintf(file, "popreg ax\n");

        node_asmer(file, node->right, f_mem);

        node_asmer(file, node->left, f_mem);

        fprintf(file, "pushreg ax\n");
        fprintf(file, "push %d\n", f_mem[func_name].all_stuff_count); //ok ff = 2, выход за границы массива
        fprintf(file, "sub\n");
        fprintf(file, "popreg ax\n");

        fprintf(file, "ret\n");

        /*
        int rr = 1; //ok что-то бессмысленно это емае
        while (f_varies[rr] != 0)
        {
            printf ("aaaaa\n");
            free(f_varies[rr++]);
            f_varies[rr] = NULL;
        }
         */

    }
    else if (node->type == 'V')
    {
        if (f_mem[func_name].point == 1)
        {
            int p = find_Fvar((char) node->value);
            fprintf(file, "pushopreg [ax+%d]\n", p - f_mem[func_name].all_stuff_count);
            if (node->right) node_asmer(file, node->right, f_mem);
        }

        else
        {
            fprintf(file, "popopreg [ax+%d]\n", q);
            f_varies[glav_q + q - 1] = (vari *) calloc(1, sizeof(vari));
            f_varies[glav_q + q - 1]->name = (char) node->value;
            f_varies[glav_q + q - 1]->pos = q;
            //glav_q = q;
            q++;
            if (node->right) node_asmer(file, node->right, f_mem);
        }
    }
    else if (node->type == 'P')
    {
        node_asmer(file, node->left, f_mem);
        node_asmer(file, node->right, f_mem);

    }
    else if (node->type == 'f')
    {
        //func_name = (int) node->value;

        if (node->left)
        {
            node = node->left;
            while (node->right) node = node->right;
            node_asmer(file, node, f_mem);
        }

        while (node->parent->type != 'f') node = node->parent;

        node = node->parent;

        fprintf(file, "call :%d\n", (int) node->value);

        if (node->right)
        {
            node = node->right;
            while (node->right) node = node->right;
            node_asmer(file, node, f_mem);
        }

    }
    else if (node->type == 'p') // параметры в функцию
    {
        int p = find_var((char) node->value);
        if (func_name == count_func - 1)
        {

            if (p != -1)        fprintf(file, "pushop %d\n", p);
            else                printf("op, varies error\n");
        }
        else
        {
            p = find_Fvar((char) node->value);
            fprintf(file, "pushopreg [ax+%d]\n", p - f_mem[func_name].all_stuff_count);
        }

        if (node->parent->type == 'p') node_asmer(file, node->parent, f_mem);
    }
    else if (node->type == 'u') // положить в параметры
    {
        int p = find_var((char) node->value);
        if (func_name == count_func - 1)
        {

            if (p != -1)        fprintf(file, "popop %d\n", p);
            else                printf("op, varies error\n");
        }
        else
        {
            p = find_Fvar((char) node->value);
            fprintf(file, "popopreg [ax+%d]\n", p - f_mem[func_name].all_stuff_count);
        }

        if (node->parent->type == 'u') node_asmer(file, node->parent, f_mem);
    }
    else if (node->type == 'o')
    {
        switch ((int) node->value)
        {
            case 'i': //if

                node_asmer(file, node->left, f_mem);
                fprintf(file, "jmp :%d\n", targ + 1);
                fprintf(file, ":%d\n", targ);
                targ++;
                int targ2 = targ;
                node_asmer(file, node->right, f_mem);

                fprintf(file, ":%d\n", targ2);

                break;
            case 'w':

                fprintf(file, ":%d\n", targ);
                int targ1 = targ;

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "jmp :%d\n", targ1);
                fprintf(file, ":%d\n", targ);
                targ++;

                break;

            case 'I':

                fprintf (file, "in\n");

                break;

            case 'O':

                node_asmer(file, node->left, f_mem);
                fprintf(file, "out\n");

                break;

            case 'e': // equal

                targ++;

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "je :%d\n", targ);
                //targ++;

                break;

            case '<':
                targ++;

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "ja :%d\n", targ);
                //targ++;

                break;

            case '>':
                targ++;

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "jb :%d\n", targ);
                //targ++;

                break;

            case '+':

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "add\n");

                break;

            case '-':

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "sub\n");

                break;

            case '*':

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "mul\n");

                break;

            case '/':

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "div\n");

                break;

            case '=': //ok по-хорошему тут нужна такая же проверка для p как и в 'v'

                node_asmer(file, node->right, f_mem);

                int p = 0;

                if (func_name == count_func - 1)
                {
                    p = find_var((char) node->left->value);

                    if (p != -1)        fprintf(file, "popop %d\n", p);
                    else
                    {
                        /*
                        p = find_Fvar((char) node->value);
                        fprintf(file, "pushopreg [ax+%d]\n", p - f_mem[func_name].count_params);
                         */
                        printf("op, varies error\n");
                    }
                }
                else
                {
                    p = find_Fvar((char) node->left->value);
                    fprintf(file, "popopreg [ax+%d]\n", p - f_mem[func_name].all_stuff_count);
                }

                break;
            case 'S':

                node_asmer(file, node->right, f_mem);

                fprintf(file, "sqrt\n");

                break;

            case '!':

                node_asmer(file, node->left, f_mem);
                node_asmer(file, node->right, f_mem);

                fprintf(file, "jne :%d\n", targ);

                break;
            case 'r':
                if (func_name != count_func - 1) //ok снова беспонтовое сравнение с мэйном
                {
                    f_varies[glav_q + q - 1] = (vari *) calloc (1, sizeof(vari));
                    f_varies[glav_q + q - 1]->name = (char) node->left->value;
                    f_varies[glav_q + q - 1]->pos = q;
                    q++;
                }
                else
                {
                    varies[var_pos] = (vari *) calloc (1, sizeof(vari));
                    varies[var_pos]->name = (char) node->left->value;
                    varies[var_pos]->pos = var_pos;
                    var_pos++;
                }
                node_asmer(file, node->right, f_mem);

                break;
            default:

                fprintf(file, "%c compilation error\n", node->value);

        }
    }
    else if (node->type == 'c')
    {
        fprintf(file, "push %d\n", (int) node->value);
    }
    else if (node->type == 'v')
    {
        int p = find_var((char) node->value);
        if (func_name == count_func - 1) //ok видимо, это имя функции в которой мы сейчас и мы сравниваем с мэйном ()все хорошо, потом нужно менять значение
        {

            if (p != -1)        fprintf(file, "pushop %d\n", p);
            else
            {
                /*
                p = find_Fvar((char) node->value);
                fprintf(file, "pushopreg [ax+%d]\n", p - f_mem[func_name].count_params);
                 */
                printf("op, varies error\n");
            }
        }
        else
        {
            p = find_Fvar((char) node->value);
            fprintf(file, "pushopreg [ax+%d]\n", p - f_mem[func_name].all_stuff_count); //ok p - номер переменной, вычитаем колво параметров функции ?? а может норм
        }

    }
    else
    {
        fprintf(file, "%c compilation error\n", node->type);
    }

    return;
}

int find_var(char name)
{
    int i = 0;
    if (varies[i] == NULL) return -1;
    while (name != varies[i]->name)
    {
        i++;
        if (varies[i] == NULL) return -1;
    }
    return i;
}

int find_Fvar(char name)
{
    int i = 0;
    while (name != f_varies[i]->name)
    {
        i++;
        //if (!f_varies[i]) return -1;
    }
    return f_varies[i]->pos;
}