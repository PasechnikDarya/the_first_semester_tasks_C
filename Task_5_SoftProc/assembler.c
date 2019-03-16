//
// Created by penguin on 1.11.18.
//

#include "asm.h"

int main ()
{
    FILE *file_in = fopen (name_file_in, "r");
        if (file_in == NULL)
    {
        PRINTF ("Opening file failure\n");
        ERROR_CODE = F_OPEN_ERROR;

        return 1;
    }
    unsigned int num_lines = line_count (file_in); //число строк в file_in
    PRINTF ("num_lines: %d\n", num_lines);

    char *buf = (char *) calloc(BUF_SIZE, sizeof(char)); //для текущего слова
    double *data = (double *) calloc (MAX_DATA, sizeof(*data)); //байт-код
    int *lables = (int*) calloc (NUM_LEBELS, sizeof(*lables)); // для меток
    
	assert (buf);
	assert (data);
	assert (lables);
	
    jmp_check (file_in, data, lables, buf, num_lines);
	compilation (file_in, data, lables, buf, num_lines);
	put_data(data);
	
	
    free (buf);
    free (data);
    free (lables);

    return 0;
};

void jmp_check (FILE *file_in, double *data, int *lables, char * buf, unsigned int num_lines) //только для меток
{
    for (int i = 0, cur_index = 0; i < num_lines; i++)
    {
		PRINTF ("I'M IN FOR, TIME: %d\n", i);

        fgets (buf, MAX_LINE_LENGTH, file_in);

        if (buf[0] == ':') 
        {
			buf[0] = '0';
			lables[strtol(buf, NULL, 10)] = cur_index; 
			//PRINTF ("buf[1] : %c,lables %d\n", buf[1], lables[strtol(buf, NULL, 10)]);
			continue;
		}
		
        char com[10] = "";
		char arg[10] = "";
		
        sscanf (buf, "%s %s", com, arg);
        //printf ("%s %s %d\n", com, arg, cur_index);
        if (strcmp (com, "")) cur_index++;
        if (strcmp (arg, "")) cur_index++;
    }
    /*for (int i = 0; i < num_lines; i++)
    {
		printf ("%d ", lables[i]);
	}
	printf("\n");*/
    fseek(file_in, 0, SEEK_SET);
};

void compilation (FILE *file_in, double *data, int *lables, char *buf, unsigned int num_lines)
{
	for (int i = 0, cur_index = 0; i < num_lines; i++)
    {
		PRINTF ("I'M IN FOR, TIME: %d\n", i);
		
        fgets (buf, MAX_LINE_LENGTH, file_in);

        if (buf[0] == ':') continue;
        
        char com[10] = "";
		char arg[10] = "";
        sscanf (buf, "%s %s", com, arg);
        
        #define DEF_CMD(name, num, ref) \
        { \
			assert (strcmp(com, "")); \
            if (!strcmp(com, #name)) \
            { \
                data[cur_index++] = CMD_##name; \
                if (!strcmp(com, "meow")) \
                { \
					data[cur_index] = cur_index; \
					data[++cur_index] = num_lines; \
				} \
                continue; \
            } \
        };
        
        #include "commands0_1.h"
        #undef DEF_CMD

		#define DEF_CMD(name, num, ref) \
        { \
			assert (strcmp(com, "")); \
			assert (strcmp(arg, "")); \
			\
            if (!strcmp(com, #name)) \
            { \
				data[cur_index++] = CMD_##name; \
				if (arg[0] == ':') \
				{ \
					arg[0] = '0'; \
					data[cur_index++] = lables[strtol(arg, NULL, 10)]; \
				} \
				else if (isdigit(arg[0])) \
				{ \
					data[cur_index++] = strtol(arg, NULL, 10); \
				} \
				else if (arg[0] == '[') \
				{ \
					if ('a' <= arg[1] && arg[1] <= 'z') data[cur_index++] = arg[1] - 'a' + 1; \
					assert (arg[3] == '+'); \
					int i = 4; \
					char val[10] = "0"; \
					while (arg[i] != ']') \
					{ \
						val[i-4] = arg[i]; \
						printf ("%s\n", val);\
						i++; \
					} \
					if (isdigit(arg[4])) \
					{ \
						data[cur_index++] = strtol(val, NULL, 10); \
						printf ("%s\n", val); \
					} \
					else printf ("op, error\n"); \
				} \
				else if (arg[0] == 'A')\
				{\
					data[cur_index++] = 0;\
				}\
				else \
				{ \
					if ('a' <= arg[0] && arg[0] <= 'z') data[cur_index++] = arg[0] - 'a' + 1; \
					/*if      (arg[0] == 'a') data[cur_index++] = 1; \
					else if (arg[0] == 'b') data[cur_index++] = 2; \
					else if (arg[0] == 'c') data[cur_index++] = 3; \
					else if (arg[0] == 'd') data[cur_index++] = 4; \*/\
				} \
				continue; \
            } \
        };
        
        #include "commands1_1.h"
        #undef DEF_CMD
    }
    fclose(file_in);
    PRINTF ("compilation is success\n"); 
}
void put_data (double *data)
{
	
	FILE *file_out = fopen ("file_out.txt", "wb");
	if (file_out == NULL)
	{
		PRINTF ("Opening file failure\n");
		ERROR_CODE = F_OPEN_ERROR;
		return;
	}
	/*for (int i = 0; i <= MAX_DATA; i++) {
		//fprintf (file_out, "%d", data[i]);
		PRINTF ("%lg ", data[i]); }*/

	fwrite (data, sizeof(double), MAX_DATA, file_out);

	fclose(file_out);
};

unsigned int line_count (FILE * file)
{
    int num_lines = 0;
    char tmp[LINES_MAXSIZE];
    while (fgets (tmp, LINES_MAXSIZE, file) != NULL)
        num_lines++;

    fseek(file, 0, SEEK_SET);
    return num_lines;
}
