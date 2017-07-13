
#ifndef TEXT_I
#define TEXT_I

typedef struct node
{
char word[100];
int count[10];
struct node *next;
}Node_t;

typedef struct hash
{
struct hash *head;
}Hash_t;


int find_key_ti(int ch);

Node_t *create_hash_node_ti(char *word, int idx, int argc);

void insert_to_hash_ti(int key, char *buff, int idx, Hash_t *hash_table, int argc);

int create_data_base(char *argv[], int idx, FILE *ptr, Hash_t *hash_table, int argc);

int open_file_ti(int argc, char *argv[], Hash_t *hash_table);

Node_t *search_word_ti(char *word, Hash_t *hash_table, char *argv[], int argc);

void delete_word(Hash_t *hash_table, char *word, int key, int choice, int argc, char *argv[]);

void edit_data_base(Hash_t *hash_table, char *word, char *buff, int key, int choice, int argc, char *argv[]);

void create_backup(Hash_t *hash_table, int argc, char *argv[]);
int update_data_base(Hash_t **hash_table, int *argc, char *argv[]);
void print_hash_table(Hash_t *hash_table);
#endif
