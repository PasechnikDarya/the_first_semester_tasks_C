#ifndef MY_PROJECT_STACK_H
#define MY_PROJECT_STACK_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define RESET   "\033[0m"
#define RED     "\a\033[1;31m"
#define GREEN   "\033[1;32m"
#define BLUE    "\033[1;36m"

const double PRECISION = 0.0000001;
const int EMPTY = 666666;

const double VAL_CANARY1 = 1.234;
const double VAL_CANARY2 = 12.34;
const double VAL_CANARY3 = 123.4;
const double VAL_CANARY4 = 1234;

enum ERROR_CODE
{
	NO_ERROR = 0,           ///< The case of normal situation
    ALLOC_ERROR = 1,        ///< The case of Allocation failure
    NULL_POINTER_ERROR = 2, ///< The case of NULL pointer
	UNDERFLOW_ERROR = 3,    ///< The case of empty stack
	STACK_SIZE_ERROR = 4,   ///< The case of size failure
	CANARY_ERROR = 5,       ///< The case of canariy`s values are wrong
	HASHSUM_ERROR =6        ///< The case of hashsum value is wrong 
};

const int   INITIAL_CAPACITY = 100; 
const float MULTIPLIER = 2;
const float DIVIDER = 1.2;

typedef double data_t;

typedef struct stack_t
{
	double canary1;
	data_t *data;
	double canary2;
	size_t size;
	double canary3;
	size_t capacity;
	double canary4;
	int ERROR_CODE;
	data_t hashsum;
} stack_t;

/**
*   \brief The function for creating stack
*   @param [in] s Pointer to stack struct 
*   @note ERROR_CODE = ALLOC_ERROR in case failure to issue memory
*/

void stack_Ctor (stack_t *s);

/**
    \brief The function for delete stack
    @param [in] s Pointer to stack struct 
*/

void stack_Dtor (stack_t *s);

/**
    \brief Verifier function for stack
    @param [in] s Pointer to stack struct 
*/

int is_stack_ok (stack_t *s);

/**
    \brief The function for memory data realocation  
    @param [in] s Pointer to stack struct 

    @note ERROR_CODE = ALLOC_ERROR in case failure to issue memory
*/

void stack_increase_size (stack_t *s);

/**
    \brief The function for memory data realocation  
    @param [in] s Pointer to stack struct 

    @note ERROR_CODE = ALLOC_ERROR in case failure to issue memory
*/

void stack_reduce_size (stack_t *s);

/**
    \brief The function for pushing element to stack 
    @param [in] s Pointer to stack struct 
*/

void stack_push (stack_t *s, data_t value);

/**
    \brief The function for poping element from stack 
    @param [in] s Pointer to stack struct 

    @note ERROR_CODE = UNDERFLOW_ERROR in case of empty stack
*/

data_t stack_pop (stack_t *s);

/**
    \brief Verifier function for canaries 
    @param [in] s Pointer to stack struct 

    @note Return ERROR_CODE = CANARY_ERROR in case of wrong canaries values 
*/

int canary_error(stack_t *s);

/**
    \brief Comparing for doubles 
    @param [in] a double 
    @param [in] b double
*/

int is_equal(double a, double b);

/**
    \brief Function for counting hashsum 
    @param [in] s Pointer to stack struct 
*/

data_t count_stack_hashsum (stack_t *s);

void unittest0 ();
void unittest1 ();
void unittest2 ();
void unittest3 ();
void unittest4 ();
void unittest5 ();
void unittest6 ();
void unittest7 ();
void unittest8 ();
void unittest9 ();
void unittest10 ();

void printf_type (char *a, void *arg);
#endif
