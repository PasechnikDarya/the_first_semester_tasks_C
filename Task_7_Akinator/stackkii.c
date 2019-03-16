#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

const double PRECISION = 0.0000001;

const int   INITIAL_CAPACITY = 100;

typedef char * data_t;

typedef struct stack_t
{
	data_t *data;
	size_t size;
	size_t capacity;
} stack_t;

void stack_Ctor (stack_t * s)
{	
	s->capacity = INITIAL_CAPACITY;
	s->data = (data_t *) calloc (s->capacity, sizeof(data_t));  
	s->size = 0;

}

void stack_Dtor(stack_t *s)
{
	memset(s->data, 0, s->size);
	free(s->data);
	s->data = NULL;
	s->size = -1ll;
	s->capacity = -1ll;
	
}

void stack_push (stack_t *s, data_t value)
{
	s->data[s->size++] = value;
}

data_t stack_pop (stack_t *s)
{
    if (s->size == 0)
    {
		return NULL;
	}
	
    data_t last_elem = s->data[--(s->size)];
    s->data[s->size] = 0;

    return last_elem;
}
