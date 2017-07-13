

#include <stdio.h>
#include <text_index.h>
#include <stdlib.h>

/*main function block*/
int main(int argc, char *argv[])
{

	/*loval variable declaration*/
	char word[100], buff[100];
	int choice, idx, flag = 0, backup_flag = 0;
	Hash_t *hash_table = NULL;

	do 
	{

		/*user menu*/
		printf("\nChoose the operation : \n\n\t1.Create DataBase\n\t2.Search\n\t3.Delete\n\t4.Edit\n\t5.Create Backup\n\t6.Update DataBase\n\t7.Print hash_table\n\t8.Exit\n\nEnter your choice : ");
		scanf("%d", &choice);

		switch (choice)
		{

			/*create database case*/
			case 1:
				{

					/*check if database is already created*/
					if (backup_flag || flag)
					{
						printf("\n\n\tDatabase Already created\n\n");
						break;
					}
					/*create hash table*/
					hash_table = calloc(26, sizeof (Hash_t));
					if (open_file_ti(argc, argv, hash_table))
					{
						flag = 1;
					}
				}
				break;

				/*search case*/
			case 2:
				{

					/*check if database is available*/
					if (!flag && !backup_flag)
					{
						printf("\n\n\tError !!! Data base not available\n\n");
						break;
					}

					/*read the word to be searched for*/
					printf("\nEnter the word to be searched for : ");
					scanf("\n%[^\n]", word);

					Node_t *ptr = NULL;

					/*calling search word function*/
					if (ptr = search_word_ti(word, hash_table, argv, argc))
					{
						printf("\n\t\tMATCH FOUND\n\n");

						for (idx = 1; idx < argc; idx++)
						{
							printf("\t%d occurances in %s file\n", ptr->count[idx - 1], argv[idx]);
						}
					}

					/*error handling if word not found*/
					else
					{
						printf("\n\t\tMATCH NOT FOUND\n\n");
					}

				}
				break;

				/*delete case*/
			case 3:
				{
					/*check if database is available*/
					if (!flag && !backup_flag)
					{
						printf("\n\n\tError !!! Data base not available\n\n");
						break;
					}

					/*read the word to be deleted from database*/
					printf("\nEnter the word to be deleted from database : ");
					scanf("\n%[^\n]", word);

					/*find the key*/
					int key = find_key_ti(word[0]);

					/*file options to choose from*/
					printf("Choose the file to delete from : \n\n");

					for (idx = 1; idx < argc; idx++ )
					{
						printf("\t%d.%s\n", idx, argv[idx]);
					}

					printf("\t%d.All Files\n\n", idx);

					/*read the choice*/
					printf("Enter the choice : ");
					scanf("%d", &choice);

					/*calling delete word function block*/
					delete_word(hash_table, word, key, choice, argc, argv);
				}
				break;

				/*edit case*/
			case 4:
				{
					/*check if database is available*/
					if (!flag && !backup_flag)
					{
						printf("\n\n\tError !!! Data base not available\n\n");
						break;
					}

					/*read the word to be edited*/
					printf("\nEnter the word to be edited from database : ");
					scanf("\n%[^\n]", word);

					/*read the replacement word*/
					printf("\nEnter the word to be inserted : ");
					scanf("\n%[^\n]", buff);

					/*calling find key*/
					int key = find_key_ti(word[0]);

					printf("Choose the file to edit from : \n\n");

					for (idx = 1; idx < argc; idx++ )
					{
						printf("\t%d.%s\n", idx, argv[idx]);
					}
					printf("\t%d.All Files\n\n", idx);

					printf("Enter the choice : ");
					scanf("%d", &choice);

					/*calling edit function block*/
					edit_data_base(hash_table, word, buff, key, choice, argc, argv);
				}
				break;

				/*create backup case*/
			case 5:
				{
					/*check if database is available*/
					if (flag || backup_flag)
					{
						create_backup(hash_table, argc, argv);
					}

					else
					{
						printf("\n\n\t\tError !!! Nothing in database to create backup. Use the option 1 to Create a database\n\n");
					}
				}
				break;

				/*update data base case*/
			case 6:
				{
					if (update_data_base(&hash_table, &argc, argv))
						backup_flag = 1;
				}
				break;

				/*print hash table case*/
			case 7:
				{
					print_hash_table(hash_table);
				}
				break;

				/*exit case*/
			case 8:
				{
					exit(1);
				}
				break;
		}
	} while (1);
}
