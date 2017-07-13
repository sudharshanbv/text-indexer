
#include <stdio.h>
#include <text_index.h>
#include <string.h>
#include <stdlib.h>
/*---------------------------------------------------------------------------------------------------------------------*/
/*print hah table function block*/
void print_hash_table(Hash_t *hash_table)
{

	/*local variable declaration*/
	int idx;
	Node_t *ptr;

	/*error handling if hash table does not exist*/
	if (hash_table == NULL)
	{
		printf("\n\n\tError !!! Hash Table not created\n\n");
		return;
	}

	/*loop to print the hash table*/
	for (idx = 0; idx < 26; idx++)
	{
		if (hash_table[idx].head != NULL)
		{
			ptr = (Node_t *)hash_table[idx].head;

			while (ptr != NULL)
			{
				printf("%s  ", ptr->word);
				ptr = ptr->next;
			}
			printf("\n\n");
		}
	}

}

/*---------------------------------------------------------------------------------------------------------------------*/
/*update data base function block*/
int update_data_base(Hash_t **hash_table, int *argc, char *argv[])
{


	/*local variable declaration*/
	FILE *ptr_bck = NULL;
	int num, idx, old_argc, bytes_read, new_argc, key, jdx;
	char buff[100], ch = ':';
	Node_t *ptr = NULL;

	/*if hash table is not present, create the hash table*/
	if (*hash_table == NULL)
	{
		*hash_table = calloc(26, sizeof (Hash_t));
	}

	/*open the back up file*/
	ptr_bck = fopen("Backup.txt", "r");

	/*error handling for file open failure*/
	if (ptr_bck == NULL)
	{
		printf("\n\n\tError!!!Backup.txt file open operation failed, Backup file not found\n");
		return 0;
	}

	fseek(ptr_bck, 0, SEEK_END);

	/*finding tge size of the file*/
	long long int eof = ftell(ptr_bck) ;	

	/*if there is content in the file*/
	if (ftell(ptr_bck))
	{

		/*initializing the file pointer to the start*/
		fseek(ptr_bck, 0, SEEK_SET);

		/*saving the current argc value*/
		old_argc = *argc;

		fread(&num, 4, 1, ptr_bck);

		fseek(ptr_bck, 1, SEEK_CUR);

		/*saving the argc val in the backup file*/
		new_argc = num;

		*argc = *argc + num - 1;

		for (idx = old_argc; idx < *argc; idx++)
		{
			argv[idx] = malloc(60);
			fread(argv[idx], 50, 1, ptr_bck);

			if (old_argc > 1)
			{
				for (jdx = 1; jdx < old_argc; jdx++)
				{
					if (strcmp(argv[idx], argv[jdx]) == 0)
					{
						printf("\n\n\t\tError!!! \"%s\" file is already entered through command line\n\n", argv[idx]);
						*argc = old_argc;
						fclose(ptr_bck);
						return 0;
					}

				}


			}

			fseek(ptr_bck, 1, SEEK_CUR);
		}

		fseek(ptr_bck, 1, SEEK_CUR);

		/*loop till end of file*/
		while (ftell(ptr_bck) < eof)
		{

			/*reading the key*/
			fread(&key, 4, 1, ptr_bck);
			fseek(ptr_bck, 1, SEEK_CUR);

			/*my each line ended by # symbol*/
			while (ch != '#')
			{
				fread(buff, 50, 1, ptr_bck);
				fseek(ptr_bck, 1, SEEK_CUR);
				
                                /*calling insert to hash table function*/
				insert_to_hash_ti(key, buff,  0, *hash_table, *argc);

				/*calling the search word function block*/
				ptr = search_word_ti(buff, *hash_table, argv, *argc);
                                
                               /*loop to increase the count*/
				for (idx = old_argc - 1; idx < *argc - 1; idx++)
				{
					fread(&num, 4, 1, ptr_bck);
					ptr->count[idx] = num;
					fseek(ptr_bck, 1, SEEK_CUR);
				}
				fread(&ch, 1, 1, ptr_bck);

				if (ch != '#')
				{
					fseek(ptr_bck, -1, SEEK_CUR);
				}
			}
			ch = ':';
		}
		printf("\n\t\tData Base Successfully Updated\n");

		/*closing the backup file*/
		fclose(ptr_bck);
		return 1;
	}

	/*if no data present in the backup file, error handle*/
	else
	{
		printf("\nNO DATA IS BACKED UP PREVIOUSLY\n");
		return 0;
	}
}

/*---------------------------------------------------------------------------------------------------------------------*/
/*create backup function block*/
void create_backup(Hash_t *hash_table, int argc, char *argv[])
{

	/*local variable declaration*/
	FILE *ptr_bck = NULL;
	Node_t *ptr = NULL;
	int idx, jdx;
	char ch = ':';
	char eol = '#';

	/*open the file to backup the content*/
	ptr_bck = fopen("Backup.txt", "w+");

	/*error handling if file open failed*/
	if (ptr_bck == NULL)
	{
		printf("\nBackup.txt file open failed\n");
		return;
	}

	else
	{
		/*write the value of argc to backup file*/
		fwrite(&argc, 4, 1, ptr_bck);

		/*writing the delimiter ":"*/
		fwrite(&ch, 1, 1, ptr_bck);
		/*writing all the file name that are being bacedup*/
		for (idx = 1; idx < argc; idx++)
		{
			fwrite(argv[idx], 50, 1, ptr_bck);
			/*writing the delimiter ":"*/
			fwrite(&ch, 1, 1, ptr_bck);
		}

		/*writing the delimiter "#"*/
		fwrite(&eol, 1, 1, ptr_bck);

		/*loop to travese he hash table*/
		for (idx = 0; idx < 26; idx++)
		{
			if (hash_table[idx].head != NULL)
			{

				ptr = (Node_t *)hash_table[idx].head;

				/*write the index to the file*/
				fwrite(&idx, 4, 1, ptr_bck);
				/*writing the delimiter ":"*/
				fwrite(&ch, 1, 1, ptr_bck);
				while (ptr != NULL)
				{
					/*write the word to th file*/
					fwrite(ptr->word, 50, 1, ptr_bck);
					/*writing the delimiter ":"*/
					fwrite(&ch, 1, 1, ptr_bck);

					/*write the count in each file to the backup file*/
					for (jdx = 0; jdx < argc - 1; jdx++)
					{
						fwrite(&ptr->count[jdx], 4, 1, ptr_bck);
						fwrite(&ch, 1, 1, ptr_bck);
					}
					ptr = ptr->next;
				}
				/*writing the delimiter "#"*/
				fwrite(&eol, 1, 1, ptr_bck);
			}
		}

		/*close the backup file*/
		fclose(ptr_bck);
	}
}
/*---------------------------------------------------------------------------------------------------------------------*/

/*global variable declaration*/
int num[10];

/*edit database function block*/
void edit_data_base(Hash_t *hash_table, char *word, char *buff, int key, int choice, int argc, char *argv[])
{

	/*local variable declaration*/
	int idx, flag = 0;
	Node_t *ptr = NULL;

	/*if edit in all files condition*/
	if (choice == argc)
	{

		/*calling search word function block*/
		ptr = search_word_ti(word, hash_table, argv, argc);

		/*if word not found , error handling*/
		if (ptr == NULL)
		{
			printf("\n\t\tWORD NOT FOUND\n\n");
			return;
		}

		/*if the word is found*/
		else
		{

			/*copy the value of count*/
			for (idx = 0; idx < argc - 1; idx++)
			{
				num[idx] = ptr->count[idx];
			}

			/*delete the word from the list*/
			delete_word(hash_table, word, key, choice, argc, argv);
		}

		/*insert the new word to the list*/
		find_key_ti(buff[0]);
		insert_to_hash_ti(key, buff,  choice, hash_table, argc);

	}

	/*editing the word in a particular file*/
	else
	{

		/*calling search word function*/
		ptr = search_word_ti(word, hash_table, argv, argc);

		/*if the word to be edited is not found, error handle*/
		if (ptr == NULL)
		{
			printf("\n\t\tWORD NOT FOUND\n\n");
			return;
		}

		/*if the word to be edited is found*/
		else
		{
			/*if the word is found but its count is zero in that particular file*/
			if (ptr->count[choice - 1] == 0)
			{
				printf("\n\t\tWORD NOT FOUND IN %s FILE\n\n", argv[choice]);
				return;
			}

			/*if the count of the word is not zero in thata file*/
			else
			{

				/*copy the value of counts*/
				for (idx = 0; idx < argc - 1; idx++)
				{
					num[idx] = ptr->count[idx];
				}

				/*checking if the word count of thata particular word is 0 in all other files*/
				for (idx = 0; idx < argc - 1; idx++)
				{
					if (idx == (choice - 1))
						continue;
					if (ptr->count[idx] != 0)
						flag = 1;
				}

				/*if yes, delete the word*/
				if (flag == 0)
					delete_word(hash_table, word, key, choice, argc, argv);
				flag = 0;

				/*insert te new word*/
				find_key_ti(buff[0]);
				insert_to_hash_ti(key, buff,  choice, hash_table, argc);

				/*search the word that is inserted and take care of count*/
				ptr = search_word_ti(buff, hash_table, argv, argc);

				ptr->count[choice - 1] = ptr->count[choice - 1] + num[choice - 1] - 1;

			}
		}


	}

}

/*---------------------------------------------------------------------------------------------------------------------*/
/*delete_val function block */
void delete_word(Hash_t *hash_table, char *word, int key, int choice, int argc, char *argv[])
{
	/*local variable declaration*/
	int idx, flag = 0;
	Node_t *prev, *ptr = (Node_t *) hash_table[key].head;

	/*if no list available, do error handling*/
	if (ptr == NULL)
	{
		printf("\nNO WORD MATCHED IN DATABASE TO DELETE!!!\n");
	}

	else
	{

		/*traverse through the list till finding the value or till reaching the last node*/
		while ((strcmp(ptr->word, word) != 0) && (ptr->next != NULL))
		{

			prev = ptr;
			ptr = ptr->next;

		}

		/*if the word found in the last node or in only node*/
		if (ptr->next == NULL)
		{	
			/*if the value to be deleted is in the first node*/
			if ((strcmp(ptr->word, word) == 0) && (ptr == (Node_t *)hash_table[key].head))
			{

				/*if the word is to be deleted from all files*/
				if (choice == argc)
					hash_table[key].head = (Hash_t *)ptr->next;

				/*if the word is to be deleted from a particular file*/
				else
				{
					/*if the count of the word is greater thn zero*/
					if (ptr->count[choice - 1] > 0)
					{

						/*decrese the count of the word in the particular file*/
						ptr->count[choice - 1] = ptr->count[choice - 1] - 1;

						/*after decrement check if the word count is zero in all files*/
						for (idx = 0; idx < argc - 1; idx++)
						{
							if (ptr->count[idx])
								flag = 1;
						}
						/*if yes, delete that word*/
						if (flag == 0)
							hash_table[key].head = (Hash_t *)ptr->next;


					}

					else
					{
						printf("\nNO WORD MATCHED THE SEARCH IN FILE %s\n\n", argv[choice]);
					}
				}

			}

			/*if the word is in the middel of the list*/
			else if (strcmp(ptr->word, word) == 0)
			{

				/*if the word to be deleted from all the files*/
				if (choice == argc)
					prev->next = ptr->next;

				/*if the word to be deleted from a particular file*/
				else
				{
					if (ptr->count[choice - 1] > 0)
					{
						ptr->count[choice - 1] = ptr->count[choice - 1] - 1;
						for (idx = 0; idx < argc - 1; idx++)
						{
							if (ptr->count[idx])
								flag = 1;
						}
						if (flag == 0)
							prev->next = ptr->next;
					}

					/*if word not found in that particular file, do error handling*/
					else
					{
						printf("\nNO WORD MATCHED THE SEARCH IN FILE %s\n\n", argv[choice]);
					}


				}
			}

			/*if word not found, do error handling*/
			else
			{

				printf("\n\t\tMATCH NOT FOUND\n");
				return;

			}

		}

		/*changing the link address*/
		else if (ptr == (Node_t *)hash_table[key].head)
		{

			if (choice == argc)
				hash_table[key].head = (Hash_t *)ptr->next;

			else
			{
				if (ptr->count[choice - 1] > 0)
				{
					ptr->count[choice - 1] = ptr->count[choice - 1] - 1;
					for (idx = 0; idx < argc - 1; idx++)
					{
						if (ptr->count[idx])
							flag = 1;
					}
					if (flag == 0)
						hash_table[key].head = (Hash_t *)ptr->next;
				}

				else
				{
					printf("\nNO WORD MATCHED THE SEARCH IN FILE %s\n\n", argv[choice]);
				}
			}
		}

		else
		{

			if (choice == argc)
				prev->next = ptr->next;

			else
			{
				if (ptr->count[choice - 1] > 0)
				{
					ptr->count[choice - 1] = ptr->count[choice - 1] - 1;
					for (idx = 0; idx < argc - 1; idx++)
					{
						if (ptr->count[idx])
							flag = 1;
					}
					if (flag == 0)
						prev->next = ptr->next;
				}

				else
				{
					printf("\nNO WORD MATCHED THE SEARCH IN FILE %s\n\n", argv[choice]);
				}
			}
		}
		/*free the memory*/
		if (choice == argc || flag == 0)
			free(ptr);
	}
}


/*----------------------------------------------------------------------------------------------------------*/
/*search word function block*/
Node_t *search_word_ti(char *word, Hash_t *hash_table, char *argv[], int argc)
{

	/*local variable declaration*/
	int idx, key, flag = 0;
	key = find_key_ti(word[0]);

	/*if head is NULL, do error handling*/
	if (hash_table[key].head == NULL)
	{
		return NULL;
	}

	else
	{
		Node_t *ptr = (Node_t *)hash_table[key].head;

		/*search for the word in the list*/
		while (ptr != NULL)
		{
			if (strcmp(ptr->word, word) == 0)
			{
				return ptr;
			}
			ptr = ptr->next;
		}

		/*if word not found in the list*/
		if (flag == 0)
		{
			return NULL;
		}
	}
}

/*------------------------------------------------------------------------------------------*/
/*find key function block*/
int find_key_ti(int ch)
{

	/*local variable declaration*/
	int key;

	/*finding key*/
	if (ch > 64 && ch < 91)
	{
		key = ch - 65;
	}

	else if (ch > 96 && ch < 123)
	{
		//key = ch - 97 + 26;
		key = ch - 97;
	}

	return key;
}
/*------------------------------------------------------------------------------------------*/
/*create hash node function block*/
Node_t *create_hash_node_ti(char *word, int idx, int argc)
{

	/*local variable declaration*/
	int jdx;

	/*allocate memory of struct variable size*/
	Node_t *new = malloc(sizeof(Node_t));

	/*store the value*/
	strcpy(new->word,word);

	if (idx == argc)
	{
		for (jdx = 0; jdx < argc - 1; jdx++)
			new->count[jdx] = num[jdx];
	}

	else
		new->count[idx - 1] = 1;

	/*store NULL in link variable*/
	new->next = NULL;

	/*return the address of allocated memory*/	
	return new;

}
/*--------------------------------------------------------------------------------------------*/
/*insert to hash function block*/
void insert_to_hash_ti(int key, char *buff, int idx, Hash_t *hash_table, int argc)
{

	/*local variable declaration*/
	int flag = 0, jdx;

	/*if head is NULL*/
	if (hash_table[key].head == NULL)
	{
		hash_table[key].head = (Hash_t *) create_hash_node_ti(buff, idx, argc);
		return;
	}

	Node_t *ptr = (Node_t *)hash_table[key].head;
	/*search for the word in the list*/
	while (ptr != NULL)
	{
		if (strcmp(buff, ptr->word) == 0)
		{
			if (idx == argc)
			{
				for (jdx = 0; jdx < argc - 1; jdx++)
					ptr->count[jdx] = ptr->count[jdx] + num[jdx];

				flag = 1;
				break;

			}

			/*increase the word count*/
			else
			{
				ptr->count[idx - 1] = ptr->count[idx - 1] + 1;
				flag = 1;
				break;
			}
		}
		ptr = ptr->next;
	}

	/*if word not found in the list*/
	if (flag != 1)
	{
		/*create a new node*/
		Node_t *new = create_hash_node_ti(buff, idx, argc);
		new->next = (Node_t *) hash_table[key].head;

		/*change the head position*/
		hash_table[key].head = (Hash_t *) new;
	}

}
/*-------------------------------------------------------------------------------------------------------*/
/*create data base function block*/
int create_data_base(char *argv[], int idx, FILE *ptr, Hash_t *hash_table, int argc)
{

	/*local variable declaration*/
	char ch, buff[100];
	int key, word_flag = 0, jdx = 0;

	/*loop to scan words till end of file*/
	while (feof(ptr) == 0)
	{

		fread(&ch, 1, 1, ptr);

		/*if the characters are aplhabets*/
		if (ch > 64 && ch < 91 || ch > 96 && ch < 123)
		{

			if (word_flag == 0)
			{
				key = find_key_ti(ch);
				word_flag = 1;
			}

			buff[jdx] = ch;
			++jdx;

		}

		/*if the characters is other than alphabet*/
		else
		{
			if (word_flag == 1)
			{
				buff[jdx] = '\0';
				insert_to_hash_ti(key, buff, idx, hash_table, argc);
				jdx = 0;
			}
			word_flag = 0;
		}

	}
}
/*-------------------------------------------------------------------------------------------------------*/
/*open file function block*/
int open_file_ti(int argc, char *argv[], Hash_t *hash_table)
{

	/*local variable declaration*/
	int idx;
	FILE *ptr;

	/*if the file names are entered in the command line*/
	if (argc > 1)
	{
		for (idx = 1; idx < argc; idx++ )
		{
			if (strstr(argv[idx], ".txt"))
			{
				/*open file in "r" mode*/
				ptr = fopen(argv[idx],"r");

				/*error handling for file open failure*/
				if (ptr == NULL)
				{
					printf("\nError!!! %s file cannot be opened\n", argv[idx]);
					return 0;
				}

				else
				{
					/*create data base function block*/
					create_data_base(argv, idx, ptr, hash_table, argc);
				}

			}

			/*invalid file extension error handling*/
			else
			{
				printf("\nError!!! Invalid file extension for %s file.\nFile extn should be \".txt\"\n", argv[idx]);
				return 0;
			}
		}
	}

	else
	{
		printf("\n\t\tError!!! NO files entered in command line Interface\n\nTry updating database from backup or enter file names in command line\n\n");
		return 0;
	}
}
/*------------------------------------------------------------------------------------------*/
