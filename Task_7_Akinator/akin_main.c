#include "akinator.c"

int main ()
{
	printf ("Hello, do you want to play with me?\n");
	printf ("press [y] for YES, [n] for NO\n");
	char ans = 0;
	scanf ("\n%c", &ans);
	if (ans == 'y')
	{
		FILE * file_in = fopen ("file_ak.txt", "r");
		tree_t akin;
		tree_ctor (&akin);
		akinator_read (&akin, file_in);
		//tree_print (&akin);
		//fclose (file_in);
			
		char gg = 0;
		while (gg != 'n')
		{
			printf ("I'm so happy, lets start\n");
			printf ("Press [a], if you want me to guess the character\n"); 
			printf ("Press [k], if you want to know the characteristics of a character\n"); 
			printf ("Press [c], if you want to compare characters\n");
			printf ("Press [e], if you want to exit\n");
			printf ("Press [s], if you want to see the akinator's memory\n");
			
			scanf ("\n%c", &gg);
			
			switch (gg)
			{
				case 'a':
				
					gg = game (&akin);
					break;
					
				case 'k':
					
					gg = character_describe (&akin);
					break;
					
				case 'c':
				
					gg = characters_comparing (&akin);
					break;
					
				case 'e':
				
					file_in = fopen ("file_ak.txt", "w");
					akinator_write (akin.root, file_in);
					fclose (file_in);
					
					return 0;
					break;
					
				case 's':
					
					akinator_dot (&akin);
					break;

				default:
					printf ("I don't understand..try again");
			}
		}
	}
	else if (ans == 'n')
	{
		printf ("you are so boring..\n I don't want play with you too\n");
	}
	else printf ("I don't understand..try again");
	
	return 0;
}

