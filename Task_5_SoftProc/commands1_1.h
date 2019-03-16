DEF_CMD (push, 1,
{
	
	index++;
	stack_push (&(cpu.memory), mem_data[index]);
	//printf("i pushed %lg\n", mem_data[index]);
	//(*index)++;
})
DEF_CMD (pushreg, 3,
{
	index++;
	stack_push (&(cpu.memory), (cpu.registers)[(int) mem_data[index]]);
	//printf ("i pushed to mem from reg %d what? %lg\n", (int) mem_data[index], (cpu.registers)[(int) mem_data[index]]);
})
DEF_CMD (popreg, 4,
{
	index++;
	(cpu.registers)[(int) mem_data[index]] = stack_pop (&(cpu.memory));
	//printf ("i poped to reg  %d from mem what? %lg\n", (int) mem_data[index], (cpu.registers)[(int) mem_data[index]]);

})
DEF_CMD (pushop, 5,
{
	index++;
	stack_push (&(cpu.memory), (cpu.op_mem)[(int) mem_data[index]]);
})
DEF_CMD (popop, 6, 
{
	index++;
	(cpu.op_mem)[(int) mem_data[index]] = stack_pop (&(cpu.memory));
})
DEF_CMD (pushopreg, 66,
{
	index++;
	int val = (int) (cpu.registers)[(int) mem_data[index++]];
	stack_push (&(cpu.memory), val + (int) mem_data[index]);
	printf ("pushop %d\n", val + (int) mem_data[index]);
})
DEF_CMD (popopreg, 67, 
{
	index++;
	int val = (int) (cpu.registers)[(int) mem_data[index++]];
	(cpu.op_mem)[val + (int) mem_data[index]] = stack_pop (&(cpu.memory));
})
DEF_CMD (ja, 13,
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));

	if (a > b)
	{
	int tmp = ++index;
	index = (double) mem_data[tmp] - 1;
	}
	else
	{
	index++;
	}
})
DEF_CMD (jb, 14, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));

	if (a < b)
	{
	int tmp = ++index;
	index = (double) mem_data[tmp] - 1;
	}
	else
	{
	index++;
	}
})
DEF_CMD (je, 15, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));

	if (a == b)
	{
		int tmp = ++index;
		index = (double) mem_data[tmp] - 1;
	}
	else
	{
		index++;
	}
})
DEF_CMD (jmp, 16, 
{
	int tmp = ++index;
	index = (int) mem_data[tmp] - 1;
})
DEF_CMD ( jiz, 17,
{
	double a = stack_pop (&(cpu.memory));

	if (a == 0)
	{
		int tmp = ++index;
		index = (double) mem_data[tmp] - 1;
		//printf ("you have zeroo\n");
	}
	else
	{
		index++;
	}
})

DEF_CMD (call, 18,
{
	stack_push (&(cpu.func), index + 1);
	
	int tmp = ++index;
	index = (double) mem_data[tmp] - 1;
	
})

DEF_CMD (jac, 25,
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));

	if (a >= b)
	{
	int tmp = ++index;
	index = (double) mem_data[tmp] - 1;
	}
	else
	{
	index++;
	}
})
DEF_CMD (jbc, 26, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));

	if (a <= b)
	{
	int tmp = ++index;
	index = (double) mem_data[tmp] - 1;
	}
	else
	{
	index++;
	}
})
DEF_CMD (jne, 27, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));

	if (a != b)
	{
	int tmp = ++index;
	index = (double) mem_data[tmp] - 1;
	}
	else
	{
	index++;
	}
})
