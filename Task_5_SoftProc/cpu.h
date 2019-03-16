#include "/home/penguin/Документы/институт/1_курс/программирование/3 работа/3 работа 1.4.c"


//#define MEOW

#ifdef MEOW
#define PRINTF printf
#else
#define PRINTF ;
#endif

const char *name_file_out = "file_out.txt";

const int NUM_LABLES = 512;
const int NUM_REG = 30;
const int NUM_MEM = 500;
const int MEMORY_SIZE = 1000;

typedef struct cpu_t
{
	stack_t   memory;
	stack_t   func;
	
	double *op_mem;
	double *registers;
} cpu_t;

void cpu_Ctor (cpu_t *s);
void cpu_Dtor (cpu_t *s);
/*
//добавляем функции
#define DEF_CMD( name, num, ref ) \
void cpu_##name(cpu_t *cpu, double *mem_data, int *index, int *run) ref;

#include "commands0_1.h"
#include "commands1_1.h"

#undef DEF_CMD
*/
void running (const char *name_file_out);
int get_num_lines (double *mem_data);
int get_num_comms (double *mem_data);
double sQrt (double a);
