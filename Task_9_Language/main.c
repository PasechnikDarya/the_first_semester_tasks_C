//
// Created by penguin on 20.12.18.
//
#include "head.h"
#include "langu_tree.c"
#include "tree_cr.c"
#include "asm_cr.c"


char *file_asm = "file_asm.txt";
int f_size (FILE* file);

const int MAX_SPUSK_FILE_SIZE = 1000;
const int NUM_FUNCS = 10;

int main ()
{

    tree_t *equ = (tree_t *) calloc (1, sizeof (*equ));
    tree_ctor (equ);

    FILE * file_in = fopen ("/home/fox/Документы/1_курс/1_семестр/программирование/Language/file_1.txt", "rb");
    if (file_in == NULL)
    {
        printf("Do not open file\n");
        return -1;
    }
    printf ("open\n");
    int size = f_size(file_in);

    func *f_mem = (func *) calloc(NUM_FUNCS, sizeof(func));


    char *buf = (char *) calloc (size, sizeof(*buf));
    fread (buf, size, sizeof(*buf), file_in);

    printf ("create\n");
    node * rez = getNG (buf, equ, f_mem);

    //tree_print (equ);
    //tree_koncole (equ->root);
    tree_dot_print (equ);

    tree_asmer(file_asm, equ, f_mem);

    //tree_dot_print (equ);

    tree_dtor (equ);
    return 0;
}