#ifndef DLIST_T
#define DLIST_T

typedef struct node
{

int data;
struct node *next, *prev;

}Dlist_t;

void slice(char *expression, Dlist_t **var1, Dlist_t **var2);

Dlist_t *apc_evaluation_sum(Dlist_t **var1, Dlist_t **var2, char operator);

Dlist_t *apc_evaluation_mul(Dlist_t **var1, Dlist_t **var2, char operator);

void apc_evaluation_power(Dlist_t **var1, Dlist_t **var2);

int apc_evaluation_div(Dlist_t **var1, Dlist_t **var2, char operator);

int check_greater(Dlist_t **var1, Dlist_t **var);

Dlist_t *dl_create_node(int val);

void dl_insert_first(Dlist_t **head, int val);

void dl_insert_last(Dlist_t **head, int val);

void dl_insert_after(Dlist_t **head,int element, int val);

void dl_insert_before(Dlist_t **head,int element, int val);

void dl_insert_nth(Dlist_t **head,int pos, int val);

void dl_print_list(Dlist_t *head);

void dl_delete_first(Dlist_t **head);

void dl_delete_last(Dlist_t **head);

void dl_delete_node(Dlist_t **head, int val);

void dl_delete_list(Dlist_t **head);
#endif
