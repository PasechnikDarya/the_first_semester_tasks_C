#include "cpu.h"
//#include <math.h>

int main ()
{
	//printf("hello\n");
	running(name_file_out);

	return 0;
}
void running(const char *name_file_out)
{
	PRINTF ("i'm running\n");
	FILE *file_data = fopen (name_file_out, "rb");
	if (file_data == NULL)
	{
		PRINTF ("Opening file failure\n");
		return;
	}

	
	PRINTF	("i read the file, look\n");
	cpu_t cpu = {0};
	cpu_Ctor (&cpu);
	
	PRINTF ("wow, i created cpu\n");
	
	double *mem_data = (double *) calloc (MEMORY_SIZE, sizeof(*mem_data));
	
	PRINTF ("that was calloc and i'm still alive\n");
	
	fread (mem_data, sizeof (double), MEMORY_SIZE, file_data);

	int num_lines = get_num_lines (mem_data);
	int num_comms = get_num_comms (mem_data);
	
	PRINTF ("num_lines: %d,\nokey.. i read from your file now, it is there:\n", num_lines);
	//for (int i = 0; i < num_comms; i++) printf("\t%lg\n", mem_data[i]);
	
	int run  = 1;
	int index = 0;
	while (run)
	{
		int cmd = mem_data[index];
		
		switch (cmd)
		{
			#define DEF_CMD(name, num, ref)                                 \
			case num:                                                       \
			{                                                               \
				ref;                                                        \
				break;                                                      \
			}                                                               

			#include "commands0_1.h"
			#include "commands1_1.h"
			
			#undef CPU_DEF_CMD
			default:
				break;
		}
		index++;
	}
	//printf ("%ld\n", cpu.memory.size);
	//for (int i = 0; i < cpu.memory.size; i++) printf ("%lg ", cpu.memory.data[i]);
    
	cpu_Dtor (&cpu);

	free(mem_data);

	printf (" # Implementation was successful \n");
	
}

int get_num_lines (double *mem_data)
{
	int i = 0;
	while (mem_data[i] != 23) i++;
	return mem_data[i+2];
}

int get_num_comms (double *mem_data)
{
	int i = 0;
	while (mem_data[i] != 23) i++;
	return mem_data[++i];
}
void cpu_Ctor (cpu_t *s)
{
	assert (s);

	stack_Ctor (&(s->memory));
	stack_Ctor (&(s->func));

	s->registers = (double *) calloc (NUM_REG, sizeof (*(s->registers)));
	s->op_mem = (double *) calloc (NUM_MEM, sizeof (*(s->registers)));
	//s->lables = (data_cpu *)calloc (RAM_MEMORY_SIZE, sizeof (*(c->lables)));
}

void cpu_Dtor (cpu_t *s)
{
	assert (s);
	
    stack_Dtor (&(s->memory));

    stack_Dtor (&(s->func));


    //free(s->ram);
    free(s->op_mem);
    free(s->registers);

    //s->ram = NULL;
    s->registers = NULL;
    s = NULL;
}
/*
double sQrt (double a)
{
	return sin(a);
}
*/
