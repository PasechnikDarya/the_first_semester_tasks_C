#include "stack_head.h"

#define DEBUG

#ifdef DEBUG

#define HASH_COUNT 0
#define HASH if(HASH_COUNT)

#define CANARY_COUNT 0
#define CANARY if (CANARY_COUNT)

#define ASSERT_OK(s)                                                                             \
	if (is_stack_ok(s))                                                                          \
	{                                                                                            \
		stack_dump(*(s));                                                                        \
	}
	
#define stack_dump(s)                                                                            \
	{                                                                                            \
		if  ((s).ERROR_CODE == UNDERFLOW_ERROR)                                                  \
		{                                                                                        \
			printf ("\n----------------------------------------------\n");                       \
			printf ("WARNING: stack is empty\n\t*action failed\n\n");                            \
			printf ( " \t* ERROR_CODE %d\n"                                                        \
					 " \t* file       %s\n"                                                        \
					 " \t* line       %d\n"                                                        \
					 " \t* function   %s\n"                                                        \
					 , (s).ERROR_CODE, __FILE__, __LINE__, __FUNCTION__);                        \
			printf ("\n----------------------------------------------\n");                       \
			(s).ERROR_CODE = NO_ERROR;                                                           \
		}                                                                                        \
		else if ((s).ERROR_CODE != NULL_POINTER_ERROR && (s).ERROR_CODE != ALLOC_ERROR)          \
		{                                                                                        \
			printf ("\n----------------------------------------------\n");                       \
			printf ( " \t* ERROR_CODE %d\n"                                                        \
					 " \t* file       %s\n"                                                        \
					 " \t* line       %d\n"                                                        \
					 " \t* function   %s\n"                                                        \
					 , (s).ERROR_CODE, __FILE__, __LINE__, __FUNCTION__);                        \
			printf ("\n************* Stack situation ****************\n");                       \
			printf ("\tstack name:       %12s\n", #s);                                           \
			printf ("\tcurrent capacity: %12lu\n", (s).capacity);                                \
			printf ("\tcurrent size:     %12lu\n", (s).size);                                    \
			printf ("\tdata[%lu]:[%p]\n", (s).size, (s).data);                                   \
			printf("\n----------------------------------------------\n");                        \
			for (int i = 0; i < (s).size; ++i) printf ("%lg ", (s).data[i]);                      \
			printf ("\n----------------------------------------------\n");                       \
			HASH printf ("\nHASHSUM:     %12lg  DEFAULT: %lg\n", (s).hashsum, count_stack_hashsum (&s));\
			printf ("\n===================CANARIES===================\n");                              \
			CANARY printf ("\t*canary1:    %12lf DEFAULT: %lf\n", (s).canary1, VAL_CANARY1);            \
			CANARY printf ("\t*canary2:    %12lf DEFAULT: %lf\n", (s).canary2, VAL_CANARY2);            \
			CANARY printf ("\t*canary3:    %12lf DEFAULT: %lf\n", (s).canary3, VAL_CANARY3);            \
			CANARY printf ("\t*canary4:    %12lf DEFAULT: %lf\n", (s).canary4, VAL_CANARY4);            \
			printf ("\n----------------------------------------------\n");                              \
			exit ((s).ERROR_CODE);                                                                      \
		}                                                                                        \
		else                                                                                     \
		{                                                                                        \
			printf ("\n----------------------------------------------\n");                       \
			printf ( " \t* ERROR_CODE %d\n"                                                        \
					 " \t* file       %s\n"                                                        \
					 " \t* line       %d\n"                                                        \
					 " \t* function   %s\n\n"                                                      \
					 , (s).ERROR_CODE, __FILE__, __LINE__, __FUNCTION__);                        \
			printf ("\n----------------------------------------------\n");                       \
			exit((s).ERROR_CODE);                                                                \
		}                                                                                        \
	}                                                                                            \


#else

#define stack_dump(s) ;
#define ASSERT_OK(s) ;

#endif


//--------------------------------------------------------------------------------------------------------------------------------------
/*
int main ()
{
	stack_t s;
	stack_Ctor(&s);
	ASSERT_OK(&s);
	for (int i = 0; i < 5; i++) {
		stack_push(&s, i);
		printf("%d ", i);
	}
	printf("\n");
	
	stack_dump(s);
	
	for (int i = 0; i < 5; i++) {
		printf("%g ", stack_pop(&s));
	}
	printf("\n");
	
	stack_dump(s);	
	
	stack_Dtor(&s);
	return 0;
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------
void stack_Ctor (stack_t * s)
{	
	s->capacity = INITIAL_CAPACITY;
	s->data = (data_t *) calloc (s->capacity, sizeof(data_t));  
	s->size = 0;
	s->ERROR_CODE = 0;
	CANARY s->canary1 = VAL_CANARY1;
	CANARY s->canary2 = VAL_CANARY2;
	CANARY s->canary3 = VAL_CANARY3;
	CANARY s->canary4 = VAL_CANARY4;
	HASH   s->hashsum = 0;
	
	ASSERT_OK(s);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void stack_Dtor(stack_t *s)
{
	ASSERT_OK(s);
		
	memset(s->data, 0, s->size);
	free(s->data);
	s->data = NULL;
	s->size = -1ll;
	s->capacity = -1ll;
	
}
//--------------------------------------------------------------------------------------------------------------------------------------
void stack_increase_size(stack_t *s)
{
	ASSERT_OK(s);
	
    s->capacity *= MULTIPLIER;
    s->data = (data_t *) realloc (s->data, s->capacity * sizeof(data_t));
    if (s->data == NULL)
    {
		s->ERROR_CODE = ALLOC_ERROR;
	
		ASSERT_OK(s);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void stack_reduce_size (stack_t *s)
{
	ASSERT_OK(s);
	
    s->capacity = s->size + 1;
    s->data = (data_t *) realloc (s->data, s->capacity * sizeof(data_t));
    if (s->data == NULL)
    {
		s->ERROR_CODE = ALLOC_ERROR;
		ASSERT_OK(s);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void stack_push (stack_t *s, data_t value)
{
	ASSERT_OK(s);
	
	if (s->size >= s->capacity) stack_increase_size (s);

	s->data[s->size++] = value;
	HASH s->hashsum = count_stack_hashsum(s);
	
	ASSERT_OK(s);
}
//--------------------------------------------------------------------------------------------------------------------------------------
data_t stack_pop (stack_t *s)
{
	ASSERT_OK(s);
    if (s->size == 0)
    {
        s->ERROR_CODE = UNDERFLOW_ERROR;
		ASSERT_OK(s);
		return EMPTY;
	}	
	
	if ((double) s->capacity / s->size > DIVIDER) stack_reduce_size (s);
	
    data_t last_elem = s->data[--(s->size)];
    s->data[s->size] = 0;
    HASH s->hashsum = count_stack_hashsum(s);
    ASSERT_OK(s);
    return last_elem;
}
//--------------------------------------------------------------------------------------------------------------------------------------
int is_stack_ok (stack_t *s)
{
	assert (s);
	if (s->ERROR_CODE == UNDERFLOW_ERROR)
		return s->ERROR_CODE;
		
	CANARY {
		if (canary_error(s))
			return s->ERROR_CODE = CANARY_ERROR;
		}
	
	HASH { 
		if (count_stack_hashsum(s) != s->hashsum)
			return s->ERROR_CODE = HASHSUM_ERROR;
		}
		
	if (s->data == NULL)
		return s->ERROR_CODE = NULL_POINTER_ERROR;

	if ((s->capacity < s->size) || (s->capacity <= 0) || (s->size < 0))
		return s->ERROR_CODE = STACK_SIZE_ERROR;
	return s->ERROR_CODE;
}
//--------------------------------------------------------------------------------------------------------------------------------------
int canary_error (stack_t *s)
{
	if (s->ERROR_CODE == UNDERFLOW_ERROR) return s->ERROR_CODE;
	if (!is_equal (s->canary1, VAL_CANARY1)) return s->ERROR_CODE = CANARY_ERROR;
	if (!is_equal (s->canary2, VAL_CANARY2)) return s->ERROR_CODE = CANARY_ERROR;
	if (!is_equal (s->canary3, VAL_CANARY3)) return s->ERROR_CODE = CANARY_ERROR;
	if (!is_equal (s->canary4, VAL_CANARY4)) return s->ERROR_CODE = CANARY_ERROR;	

	return s->ERROR_CODE;
}
//--------------------------------------------------------------------------------------------------------------------------------------
int is_equal (double a, double b)
{
	if (fabs (a - b) < PRECISION) return 1;
	else return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
data_t count_stack_hashsum (stack_t *s)
{
	int hash_sum = 0;
	
	for (int i = 0, sgn = -1; i < s->size; i++, sgn *= -1)
	{
		hash_sum += sgn * s->data[i];
	}
	return hash_sum;
}
