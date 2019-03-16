#include "/home/penguin/Документы/институт/1_курс/программирование/дерево/tree.c"
#include "stackkii.c"

void akinator_read (tree_t *akin, FILE * file_in);
void akinator_write (node *akin, FILE *file);

node * search_person (tree_t * akin, char * person);
void add_person (tree_t *s, node *prev);
node * read_elem (tree_t *akin, int *i, char *buf, node * parent_node);

char game (tree_t *akin); 

char character_describe (tree_t *akin);
char characters_comparing (tree_t *akin);
node *search_node (node * tmp_node, char *person, node *rez);

void akinator_dot (tree_t *s);
void create_tree (node *tree, FILE *f);

void node_way (tree_t * s, node *pers, stack_t *way);
