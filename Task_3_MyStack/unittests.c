#include "3 работа 1.4.c"

#define UNITTEST(what, ref, type_what, type_ref)                                          \
    {                                                                                     \
        type_what tmp_what = (what);                                                      \
        type_ref tmp_ref = (ref);                                                         \
        if (tmp_what != tmp_ref) {                                                        \
            printf (RED"FAILED"RESET " %s = ", #what);                                    \
            printf_type (#type_what, (void *) &tmp_what);                                 \
            printf (" expected ");                                                        \
            printf_type (#type_ref, (void *) &tmp_ref);                                   \
            printf("\n");                                                                 \
		}                                                                                 \
        else                                                                              \
            printf (GREEN"OK"RESET " %s = ", #what);                                      \
            printf_type (#type_ref, (void *) &tmp_ref);                                   \
            printf ("\n");                                                                \
    }
int main()
{
	unittest0 ();
	unittest1 ();
	unittest2 ();
	unittest3 ();
	CANARY unittest4 ();
	HASH unittest5 ();
	//unittest6 (); /*NULL_POINTER_ERROR*/
	//unittest7 (); /*STACK_SIZE_ERROR*/
	//unittest8 (); /*CANARY_ERROR*/
	//unittest9 (); /*HASHSUM_ERROR*/
	unittest10 ();
	
	return 0;
}

void unittest0 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check stack Constructor ****\n"RESET);
	UNITTEST ((s).size, 0, int, int);
	UNITTEST ((s).capacity, INITIAL_CAPACITY, int, int);
	UNITTEST ((s).ERROR_CODE, 0, int, int);
	HASH UNITTEST ((s).hashsum, 0, data_t, int);
	printf("----------------------------------------------\n\n");
}

void unittest1 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check pop() and push() ****\n"RESET);
	for (int i = 0; i < 5; i++) stack_push (&s, i);
	for (int i = 0; i < 5; i++) UNITTEST (stack_pop(&s), 4 - i, data_t, int);
	printf("----------------------------------------------\n\n");
}

void unittest2 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check poping from empty stack ****\n"RESET);
	for (int i = 0; i < 5; i++) stack_push (&s, i);
	for (int i = 0; i < 5; i++) stack_pop(&s);
	UNITTEST (stack_pop(&s), EMPTY, data_t, int);
	printf("----------------------------------------------\n\n");
}

void unittest3 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check stack capacity ****\n"RESET);
	for (int i = 0; i < 2000; i++) stack_push (&s, i * i);
	UNITTEST ((s).capacity, 3200, int, int);
	printf("----------------------------------------------\n\n");
}

void unittest4 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check canaries ****\n"RESET);
	for (int i = 0; i < 2000; i++) stack_push (&s, i * i);
	for (int i = 0; i < 100; i++) stack_pop (&s);
	
	UNITTEST ((s).canary1, VAL_CANARY1, double, double);
	UNITTEST ((s).canary2, VAL_CANARY2, double, double);
	UNITTEST ((s).canary3, VAL_CANARY3, double, double);
	UNITTEST ((s).canary4, VAL_CANARY4, double, double);
	
	printf("----------------------------------------------\n\n");
}	

void unittest5 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check hashsum ****\n"RESET);
	for (int i = 0; i < 2000; i++) stack_push (&s, i * i);
	for (int i = 0; i < 100; i++) stack_pop (&s);
	UNITTEST ((s).hashsum, count_stack_hashsum(&s), data_t, data_t);	
	printf("----------------------------------------------\n\n");
}

void unittest6 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check NULL_POINTER_ERROR (stack data pointer == NULL)****\n"RESET);
	for (int i = 0; i < 200; i++) stack_push (&s, i * i);
	((s).data) = NULL;
	UNITTEST (stack_pop (&s), 1, data_t, int);
	printf("----------------------------------------------\n\n");
}

void unittest7 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check STACK_SIZE_ERROR ****\n"RESET);
	for (int i = 0; i < 20; i++) stack_push (&s, i * i);
	((s).size) = 1000;
	UNITTEST (stack_pop (&s), 400, data_t, int);
	printf("----------------------------------------------\n\n");
}
void unittest8 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check CANARY_ERROR ****\n"RESET);
	for (int i = 0; i < 20; i++) stack_push (&s, i * i);
	((s).canary3) = 1000;
	UNITTEST (stack_pop (&s), 400, data_t, int);
	printf("----------------------------------------------\n\n");
}

void unittest9 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check HASHSUM_ERROR ****\n"RESET);
	for (int i = 0; i < 20; i++) stack_push (&s, i*i);
	(s).hashsum = 10;
	UNITTEST (stack_pop (&s), 1, data_t, int);
	printf("----------------------------------------------\n\n");
}

void unittest10 ()
{
	stack_t s;
	stack_Ctor(&s);
	printf(BLUE"\t**** Check stack Destructor ****\n"RESET);
	for (int i = 0; i < 20; i++) stack_push (&s, i * i - 5);
	stack_Dtor (&s);
	UNITTEST ((s).data, NULL, data_t *, void *);
	UNITTEST ((s).size, -1ll, long long int, long long int);
	UNITTEST ((s).capacity, -1ll, long long int, long long int);
	printf("----------------------------------------------\n\n");
}

void printf_type (char *a, void *arg)
{
	if (!strcmp(a, "int")) printf ("%d", *(int *)arg);
	else if (!strcmp(a, "double") || !strcmp(a, "data_t") || !strcmp(a, "float")) printf ("%lg", *(double *) arg);
	else if (!strcmp(a, "long long int")) printf ("%lld", *(long long int *) arg);
}
