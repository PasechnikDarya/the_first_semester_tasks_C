DEF_CMD (mul, 7, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));
	stack_push (&(cpu.memory), a * b);
	//(*index)++;
	
})
DEF_CMD (add, 8, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));
	stack_push (&(cpu.memory), a + b);
	//(*index)++;
})
DEF_CMD (div, 9, 
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));
	stack_push (&(cpu.memory), b / a);
	//(*index)++;
})
DEF_CMD (sub, 10,
{
	double a = stack_pop (&(cpu.memory));
	double b = stack_pop (&(cpu.memory));
	stack_push (&(cpu.memory), b - a);
	//(*index)++;
})

DEF_CMD (outt, 11, 
{
	printf ("i'm in out now. Look what you have in your memory:\n\n");
	for (int i = 0; i < (cpu.memory).size; i++)
	{
		printf ("%lg   ", (cpu.memory).data[i]);
	}
	printf ("\n");
})

DEF_CMD (out, 44, 
{
	if ((cpu.memory).size-1 >= 0) printf ("%lg   ", (cpu.memory).data[(cpu.memory).size-1]);
	else printf("stack is empty\n");
	
	printf ("\n");
})

DEF_CMD (end, 12, {run = 0;})

DEF_CMD (meow, 23, )

DEF_CMD (pop, 2, 
{
	stack_pop (&(cpu.memory));
	//printf("I poped THIS: %lg\n\n\n", a); 
	//(*index)++;
})

DEF_CMD ( ret, 21,
{
	index  = stack_pop (&(cpu.func));
})

DEF_CMD ( sqr, 22,
{
  double a = stack_pop (&(cpu.memory));

  stack_push (&(cpu.memory), a * a);
  //index += 1;
})

DEF_CMD ( in, 24,
{
	double a = 0;
	scanf ("%lg", &a);
	stack_push (&(cpu.memory), a);
	//printf ("i pushed %lg\n", a);
	//index += 1;
})

DEF_CMD (sqrt, 19,
{
  double a = sqrt (stack_pop (&(cpu.memory)));

  stack_push (&(cpu.memory), a);

 })
