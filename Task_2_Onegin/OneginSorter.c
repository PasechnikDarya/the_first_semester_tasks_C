#include <stdio.h>
#include <locale.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int ERROR_CODE = 0;

enum ERROR_CODE
{
    F_OPEN_ERROR = -1, ///< The case of Opening file failure
    ALLOC_ERROR = -2, ///< The case of Allocation failure
    NULL_POINTER_ERROR = -3, ///< The case of NULL pointer
};

/**
    \brief The function for reading file with Poem to dynamic memory array and creating dynamic array with pointers to lines of Poem
    @param [in] NUM_SYMBOLS Point to the Poem`s symbols quantity (unsigned int)
    @param [in] NUM_LINES Point to the Poem`s lines quantity (unsigned int)
    @param [out] linesPointers Pointer`s Array with Pointers to lines of Poem

    @return LinesPointers
    @note Return "Opening file failure" in case of an error in opening file, ERROR_CODE = -1
*/

char **f_read (unsigned int *NUM_SYMBOLS, unsigned int *NUM_LINES);

/**
    \brief The function for creating dynamic memory array with pointers to Poem
    @param [in] file_in Point to the file with Poem
    @param [in] NUM_SYMBOLS The Poem`s symbols quantity
    @param [out] Poem Dynamic memory array with pointers to Poem

    @return Poem
    @note Return "Allocation failure" in case failure to issue memory, ERROR_CODE = -2
*/

char *createPoemArr (FILE *file_in, unsigned int NUM_SYMBOLS);

/**
    \brief The function for creating dynamic memory array with pointers to lines of Poem
    @param [in] Poem Dynamic memory array with Poem
    @param [in] NUM_LINES Poem`s lines quantity (unsigned int)
    @param [out] LinesPointers Dynamic memory array with pointers to lines of Poem

    @return LinesPointers
    @note Return "Allocation failure" in case failure to issue memory, ERROR_CODE = -2
*/

char **createLinesPointArr (char *Poem, unsigned int NUM_LINES);

/**
    \brief The function for creating new dynamic memory array and copy current array to its
    @param [in] LinesPointers Dynamic memory array with pointers to Poem
    @param [in] NUM_LINES Point to the Poem`s lines quantity (unsigned int)
    @param [out]  Dynamic memory array with pointers to lines of Poem

    @return LinesPointers
    @note Return "Allocation failure" in case failure to issue memory, ERROR_CODE = -2
*/

char **arr_copy(char **linesPointers, int NUM_LINES);

/**
    \brief The function for writing result of program to output file
    @param [in] LinesPointers Dynamic memory array with pointers to lines of Poem

    @note Return "Opening file failure" in case of an error in opening file, ERROR_CODE = -1
*/

void f_write (char **LinesPointers);

/**
    \brief The function for freeing the requested memory
    @param [in] LinesPointers Dynamic memory array with pointers to lines of Poem
*/

void mem_return (char **LinesPointers);

/**
    \brief The function for counting quantity of symbols in the file
    @param [in] file_in Point to the file with Poem
    @param [out] SymbolsCount The Poem`s symbols quantity

    @return SymbolsCount
*/

unsigned int f_size (FILE *file_in);

/**
    \brief The function for counting quantity of lines in the Poem
    @param [in] Poem Dynamic memory array with pointers to Poem
    @param [out] LinesCount The Poem`s symbols quantity

    @return LinesCount
*/

unsigned int lines_num (char *Poem);

/**
    \brief The function for abc sorting
    @param [in] a 1st element of comparing
    @param [in] b 2st element of comparing

    @note auxiliary function for qsort
*/

int cmp_abc (const void* a, const void* b);

/**
    \brief The function with abc sorting from end of file
    @param [in] a 1st element of comparing
    @param [in] b 2st element of comparing

    @note auxiliary function for qsort
*/

int cmp_end (const void* a, const void* b);
//---------------------------------------------------------------------------------------------------------------------
int main ()
{
    setlocale (LC_ALL, "Rus");

    unsigned int NUM_SYMBOLS = 0, NUM_LINES = 0;

    char **linesPointersOriginal = f_read (&NUM_SYMBOLS, &NUM_LINES);
    if (ERROR_CODE != 0) return ERROR_CODE;

    char **linesPointersAbc = arr_copy (linesPointersOriginal, NUM_LINES);
    if (ERROR_CODE != 0)return ERROR_CODE;

    qsort(linesPointersAbc, NUM_LINES, sizeof(*linesPointersAbc), cmp_abc);

    char **linesPointersEnd = arr_copy (linesPointersOriginal, NUM_LINES);
    if (ERROR_CODE != 0) return ERROR_CODE;

    qsort(linesPointersEnd, NUM_LINES, sizeof(*linesPointersEnd), cmp_end);

    f_write (linesPointersAbc);
    f_write (linesPointersEnd);
    f_write (linesPointersOriginal);
    if (ERROR_CODE != 0) return ERROR_CODE;

    mem_return (linesPointersAbc);
    mem_return (linesPointersEnd);
    mem_return (linesPointersOriginal);
    if (ERROR_CODE != 0) return ERROR_CODE;

    printf("ERROR_CODE: %d\n", ERROR_CODE);
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
char **f_read (unsigned int *NUM_SYMBOLS, unsigned int *NUM_LINES)
{
    if (NUM_SYMBOLS == NULL || NUM_LINES == NULL)
    {
        printf("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return NULL;
    }

    FILE *file_in = fopen ("file_in.txt", "r");
    if (file_in == NULL)
    {
        printf ("Opening file failure\n");
        ERROR_CODE = F_OPEN_ERROR;

        return NULL;
    }

    *NUM_SYMBOLS = f_size (file_in);
    char *Poem = createPoemArr (file_in, *NUM_SYMBOLS);
    *NUM_LINES = lines_num (Poem);

    fclose (file_in);

    return createLinesPointArr (Poem, *NUM_LINES);
}
//--------------------------------------------------------------------------------------------------------------------------------------
char *createPoemArr (FILE *file_in, unsigned int NUM_SYMBOLS)
{
    if (file_in == NULL || NUM_SYMBOLS == -1)
    {
        printf("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return NULL;
    }

    char *Poem = (char *) calloc (NUM_SYMBOLS + 1, sizeof(*Poem));
    if (Poem == NULL)
    {
        printf ("Allocation failure\n ERROR_CODE");
        ERROR_CODE = ALLOC_ERROR;

        return NULL;
    }

    fread (Poem, NUM_SYMBOLS, sizeof(*Poem), file_in);

    return Poem;
}
//--------------------------------------------------------------------------------------------------------------------------------------
char **createLinesPointArr (char *Poem, unsigned int NUM_LINES)
{
    if (Poem == NULL || NUM_LINES == -1)
    {
        printf("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return NULL;
    }

    char **linesPointers = (char **) calloc (NUM_LINES + 1, sizeof(*linesPointers));
    if (linesPointers == NULL)
    {
        printf ("Allocation failure\n ERROR_CODE");
        ERROR_CODE = ALLOC_ERROR;

        return NULL;
    }

    linesPointers[0] = Poem;
    int curSymb = 0;
    int curPointer = 1;

    for (int curSymb = 0; Poem[curSymb] != '\0'; curSymb++)
    {
        if (Poem[curSymb] == '\n')
        {
            Poem[curSymb] = '\0';
            linesPointers[curPointer++] = Poem + curSymb + 1;
        }
    }

    return linesPointers;
}
//--------------------------------------------------------------------------------------------------------------------------------------
char **arr_copy (char **linesPointers, int NUM_LINES)
{
    if (linesPointers == NULL || NUM_LINES == -1)
    {
        printf ("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return NULL;
    }

    char **newLinesPointers = (char **) calloc (NUM_LINES + 1, sizeof(*newLinesPointers));
    if (newLinesPointers == NULL)
    {
        printf ("Allocation failure\n ERROR_CODE");
        ERROR_CODE = ALLOC_ERROR;

        return NULL;
    }

    return memmove (newLinesPointers, linesPointers, NUM_LINES * sizeof(linesPointers));
}
//--------------------------------------------------------------------------------------------------------------------------------------
void f_write (char **linesPointers)
{
    if (linesPointers == NULL)
    {
        printf ("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return;
    }

    FILE *file_out = fopen ("file_out.txt", "a");
    if (file_out == NULL)
    {
        printf ("Opening file failure\n ERROR_CODE");
        ERROR_CODE = 2;
    }

    int i = 0;
    for (int i = 0; linesPointers[i] != NULL; i++)
    {
        fprintf (file_out, "%s\n", linesPointers[i]);
        printf ("%s\n", linesPointers[i]);
    }

    fprintf(file_out, "\n");
    printf("\n");

    fclose (file_out);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void mem_return (char **pointersArr)
{
    if (pointersArr == NULL)
    {
        printf ("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return;
    }

    free (pointersArr[0]);
    free (pointersArr);
}
//--------------------------------------------------------------------------------------------------------------------------------------
int cmp_abc (const void* a, const void* b)
{
    return strcmp (*(char**) a, *(char**) b);
}
//--------------------------------------------------------------------------------------------------------------------------------------
int cmp_end (const void* a, const void* b)
{
    char* str1 = *(char**) a;
    char* str2 = *(char**) b;
    int endstr1 = 0;
    int endstr2 = 0;

    while (str1[endstr1] != '\0')
        endstr1++;
    while (endstr1 >= 0)
    {
        if (ispunct(str1[endstr1]) || str1[endstr1] == '\0') endstr1--;
        else break;
    }

    while (str2[endstr2] != '\0')
        endstr2++;
    while (endstr2 >= 0)
    {
        if (ispunct(str2[endstr2]) || str2[endstr2] == '\0') endstr2--;
        else break;
    }

    if (str1[endstr1] > str2[endstr2]) return 1;
    if (str1[endstr1] <= str2[endstr2]) return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------
unsigned int lines_num (char *Poem)
{
    if (Poem == NULL)
    {
        printf("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return -1;
    }

    unsigned int NUM_LINES = 0;

    for (int tmpSymb = 0; Poem[tmpSymb] != '\0'; tmpSymb++)
    {
        if ('\n' == Poem[tmpSymb])
        {
            NUM_LINES++;
        }
    }
    return ++NUM_LINES;
}
//--------------------------------------------------------------------------------------------------------------------------------------
unsigned int f_size (FILE *file_in)
{
    if (file_in == NULL)
    {
        printf("Pointer failure\n");
        ERROR_CODE = NULL_POINTER_ERROR;

        return -1;
    }
    fseek (file_in, 0, SEEK_END);
    unsigned int symbCount = ftell (file_in);
    fseek (file_in, 0, SEEK_SET);

    return symbCount;
}

