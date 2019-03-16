#include "akinator.h"

char game (tree_t *akin)
{
	node * cur_ques = akin->root;
	for (;;) 
	{
        if (cur_ques->left == NULL)
        {
            printf("Is it %s?\n", cur_ques->value);
            printf("press [y] for YES, [n] for NO\n");
            
            break;
        }

        printf("Is it %s?\n", cur_ques->value);
        printf("press [y] for YES, [n] for NO\n");

        char ans = 0;
        scanf("\n%c", &ans);

        if      (ans == 'n') cur_ques = cur_ques->left;
        else if (ans == 'y') cur_ques = cur_ques->right;
        else 
        {
            printf("Invalid command!\n");
            continue;           
        }
    }
    char ans = 0;
    scanf("\n%c", &ans); 

    if (ans == 'n')      
    {
		add_person (akin, cur_ques);
		printf ("Okeey, you win..\nDo you want to play again?\n");
		printf("press [y] for YES, [n] for NO\n");		
	}
    else if (ans == 'y') printf("I win again! I know everything!\nDo you want to play again?\npress [y] for YES, [n] for NO\n");
    else                 printf("Invalid command!\n");
    
	//tree_print (akin);
	
	scanf("\n%c", &ans);
	
	//if (ans == 'y') game ();
	
	return ans;
}

char character_describe (tree_t *akin)
{
	stack_t way;
	stack_Ctor (&way);
	
	char ans = 0;
	char name[MAX_LINE_SIZE];
	printf ("Enter character name you choose\n");
	
	scanf ("\n%s", name);
	
	node *person = node_ctor();
	person = search_person (akin, name);
	node_way (akin, person, &way);
	
	if (person)
	{	
		printf ("Oh, it's easy: \n %s are ", name);
		for (;;)
		{
			char * a = stack_pop (&way);
			if (a == 0) break;
			printf ("%s ", a);
		}
		printf ("\n");
	}
	else
	{
		printf ("I don't know this person..\n Play with me to create it\n");
	}
	
	printf("Do you want to play again?\n");
	printf("press [y] for YES, [n] for NO\n");	
	scanf("\n%c", &ans);
		
	return ans;
	
}

node * search_person (tree_t *s, char *person)
{
	assert_tree (s);
	assert (person != NULL);
    node *tmp_node = s->root;
    //printf("search\n");
    
    node *rez = NULL;
    rez = search_node (tmp_node, person, rez);
    if (rez != NULL) return rez;
     
	return NULL;
}

node *search_node (node * tmp_node, char *person, node *rez)
{
	//printf ("searching \\|/");
	//printf ("%s   : ", tmp_node->value);
	//printf ("%d\n", strcmp (tmp_node->value, person));
	//printf ("rez   : %p\n", rez);
	
	if (rez != NULL) 		return rez;
	if (strcmp (tmp_node->value, person))
	{
		if (tmp_node->right)
		{
			rez = search_node (tmp_node->right, person, rez);
			if (rez != NULL) return rez;
			
			rez = search_node (tmp_node->left, person, rez);
			if (rez != NULL) return rez;
			return NULL;				
		}
		else if (tmp_node->left)
		{
			rez = search_node (tmp_node->left, person, rez);
			if (rez != NULL) return rez;
			return NULL;
		}
		else return NULL;
	}

	return rez = tmp_node;
}

char characters_comparing (tree_t *akin)
{
	char ans = 0;
	char name1 [MAX_LINE_SIZE];
	char name2 [MAX_LINE_SIZE];
	
	printf ("Enter name of the first character you want to compare\n");
	scanf ("\n%s", name1);
	printf ("Enter name of the second character you want to compare\n");
	scanf ("\n%s", name2);
	
	printf ("*********Comparing**********\n");
	node *person1 = node_ctor();
	person1 = search_person (akin, name1);
	
	node *person2 = node_ctor();
	person2 = search_person (akin, name2);
	
	if (!person1 && !person2)
	{
		printf ("I don't know this characters..\nPlay with me to create it\n");
	}
	else if (!person1)
	{
		printf ("I cannot compare because I don't know who %s is.. \n %s are ", name1, name2);
		
		stack_t way;
		stack_Ctor (&way);
		person2 = search_person (akin, name2);
		node_way (akin, person2, &way);
		
		for (;;)
		{
			char * a = stack_pop (&way);
			if (a == 0) break;
			printf ("%s ", a);
		}
		printf ("\n");	
	
	}
	else if (!person2)
	{
		printf ("I cannot compare because I don't know who %s is.. \n %s are ", name2, name1);
		
		stack_t way;
		stack_Ctor (&way);
		person1 = search_person (akin, name1);
		node_way (akin, person1, &way);
		
		for (;;)
		{
			char * a = stack_pop (&way);
			if (a == 0) break;
			printf ("%s, ", a);
		}
		printf ("\n");	
	}
	else
	{
		stack_t way1;
		stack_Ctor (&way1);
		person1 = search_person (akin, name1);
		node_way (akin, person1, &way1);
		
		stack_t way2;
		stack_Ctor (&way2);
		person1 = search_person (akin, name2);
		node_way (akin, person2, &way2);
		
		int k = 1;
		char * a1 = stack_pop (&way1);
		char * a2 = stack_pop (&way2);
		
		if (!strcmp (a1, a2))
			{
				printf ("%s and %s are similar in that they are ", name1, name2);
				printf ("%s ", a1);
			}
		while (k == 1)
		{
			a1 = stack_pop(&way1);
			a2 = stack_pop(&way2);
			
			if (a1 == 0 || a2 == 0) 
			{
				k = 0;
				break;
			}
			if (!strcmp (a1, a2))
			{
				printf ("%s ", a1);
			}
			else k = 0;
		}
		printf ("\n");
		
		if (a1 != 0)
		{
			printf ("But %s is ", name1);
			for (;;)
			{
				
				if (a1 == 0) break;
				printf ("%s ", a1);
				a1 = stack_pop (&way1);
			}
		}
		printf ("\n");
		
		if (a2 != 0)
		{
			printf ("And %s is ", name2);
			for (;;)
			{				
				if (a2 == 0) break;
				printf ("%s ", a2);
				a2 = stack_pop (&way2);
			}
		}		
	}
	
	printf ("\n");
	
	
	
	
	printf("\nDo you want to play again?\n");
	printf("press [y] for YES, [n] for NO\n");	
	scanf("\n%c", &ans);
		
	return ans;
	
}

void node_way (tree_t * s, node *pers, stack_t *way)
{
	node *tmp = pers->parent;
	char *not = strdup ("not");
	node *val = NULL;
	
	while (tmp->parent)
	{
		if (pers->parent == tmp)
		{
			stack_push (way, tmp->value);
			if (tmp->left == pers) stack_push (way, not);
			val = tmp;
			tmp = tmp->parent;
			continue;
		}
		stack_push (way, tmp->value);
		if (tmp->left == val) stack_push (way, not);
		val = tmp;
		tmp = tmp->parent;
	} 
	stack_push (way, tmp->value);
	if (tmp->left == val) stack_push (way, not);
		
}

void akinator_read (tree_t *akin, FILE * file_in)
{
	/*
	fseek (file_in, 0, SEEK_END);
    int s_count = ftell (file_in);
    fseek (file_in, 0, SEEK_SET);
    */
    assert_tree (akin);
	char *buf = (char *) calloc (MAX_FILE_SIZE, sizeof(*buf));
    fread (buf, sizeof(double), MAX_FILE_SIZE, file_in);
    
    int i = 0;

    akin->root = read_elem (akin, &i, buf, akin->root);
    
    //printf ("i finished reading %d\n", i);
    assert_tree (akin);
    free (buf);
    return;  
    
}

node * read_elem (tree_t *s, int *i, char *buf, node * parent_node)
{
	assert_tree (s);
	assert (i);
    assert (buf);

    node * new_node = node_ctor();
    new_node->parent = parent_node;
    s->count++;
    
    //printf ("i read la nodes %d : %c\n", *i, buf[*i]);
    
    if ('{' == buf[*i])
    {
		(*i) += 2;
	    //printf ("i found node %d : %c\n", *i, buf[*i]);
        if ('{' == buf[*i])
        {
			//printf ("i found left node %d\n", *i);
            new_node->left = read_elem (s, i, buf, new_node);
            //printf ("oooooookey\n");
            assert_tree (s);
        }
        		
        //printf ("what is your problem a?\n");
        char *question = (char *) calloc (MAX_LINE_SIZE, sizeof(*question));
        //printf ("ccalocc : %c\n", buf[*i]);
        
        int k = 0;
        while (buf[*i] != '{' && buf[*i] != '}' )
        {
			if ('\n' == buf[*i]) 
				{
					k++;
					(*i)++;
					continue;
				}
			//printf ("%d : %c\n", *i, buf[*i]);
			question[k++] = buf[(*i)++];
		}
        new_node->value = question;
        
        //printf ("it is val of node : %c\n", *new_node->value);
        //printf ("i %d\n", *i);
        if ('{' == buf[*i])
        {
			//printf ("i found right node %d\n", *i);
            new_node->right = read_elem (s, i, buf, new_node);
            //printf ("oooooookey\n");
        }
    }
    
    if ('}' == buf[*i])
    {
		(*i) += 2;
		return new_node;
	}
	
    //printf ("uuu %c\n", buf[*i]);
	
	assert_tree (s);
    return NULL;
}

void akinator_write (node *n_node, FILE *file)
{
	if (!n_node) return;
    fprintf (file, "{\n");
    
    if (n_node->left != NULL)
    {
		akinator_write (n_node->left, file);
	}
	fprintf (file, "%s\n", n_node->value);
	/*
	for (int i = 0; i != '\n'; i++)
	{
		fprintf (file, "%c", n_node->value[i]);
		printf ("%c", n_node->value[i]);
	}*/
	if (n_node->right != NULL)
    {
		akinator_write (n_node->right, file);
	}
	fprintf (file, "}\n");   

    return;
}

void add_person (tree_t *s, node *prev)
{
	assert_tree (s);
	char buf[MAX_LINE_SIZE];

    printf ("Who it is?\n");
    scanf ("%s", buf);
    char *person = strdup (buf);

    printf ("What %s is different from %s?\n", person, prev->value);
    scanf ("%s", buf);
    
    char *prev_person = prev->value;
    char *question = strdup (buf);
    prev->value = question;

    node_add(s, prev, prev_person, 'L');
    node_add(s, prev, person, 'R');
    assert_tree (s);
    return;
}

void akinator_dot (tree_t *s)
{
	FILE *file_dot = fopen("akinator.dot", "w");
	fprintf(file_dot, "digraph\n{\n");
	fprintf(file_dot, "node[shape = \"septagon\", color=\"darkgreen\",fontsize=10, style=\"filled\", fillcolor = \"#9CC98D\"];\nedge[color=\"darkgreen\",fontsize=12];\n");
	create_tree (s->root, file_dot);
	fprintf(file_dot, "}");
	fclose(file_dot);
	system("dot -Tjpg -o tree.jpg akinator.dot");
	system("xdg-open tree.jpg");
	return;	
}

void create_tree (node *s, FILE *f)
{
	if (s->left != NULL)
	{
		fprintf (f, "\t%s[shape = \"egg\", color=\"green\", style=\"filled\", fillcolor = \"white\"];\n\t%s->%s[label = \"No\"];\n", s->value, s->value, s->left->value);
		create_tree (s->left, f);
	}
	if (s->right != NULL)
	{
		fprintf (f, "\t%s[shape = \"egg\", color=\"green\", style=\"filled\", fillcolor = \"white\"];\n\t%s->%s[label = \"Yes\"];\n", s->value, s->value, s->right->value);
		create_tree (s->right, f);
	}
	return;
}

