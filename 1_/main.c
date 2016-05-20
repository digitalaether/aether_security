#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct userlinkedlist
{
	struct userlinkedlist *next;
	struct usernode *user;
};


struct usernode
{
	int userid;
	char password[20];
	char name[20];
};




int main(int argc, char *argv[])
{
	FILE *fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		fprintf(stderr,"Failed to open file \"%s\"", argv[1]);
		return EXIT_FAILURE;
	}

	ssize_t size;
	char *line = NULL;
	size_t linesize = 0;
	struct userlinkedlist * list;
	struct userlinkedlist * current;
	list = (struct userlinkedlist *)malloc(sizeof(struct userlinkedlist));
	memset((void*)list, 0, sizeof(struct userlinkedlist));
	current = list;
	while((size = getline(&line,&linesize,fp)) != -1)
	{
		int iter = 0;
		int iter2 = 0;
		int section = 0;
		char *temp;
		temp = (char *)malloc(linesize);
		memset(temp, 0, linesize);
		current->user = (struct usernode *)malloc(sizeof(struct usernode));
		memset((void *)current->user,0,sizeof(struct usernode));
		while (line[iter] != 0)
		{
			temp[iter2] = line[iter];
			if (line[iter] == ';')
			{
				temp[iter2]=0;
				if (section == 0)
				{
					printf("userid = %d\n",atoi(temp));
					current->user->userid = atoi(temp);
					section++;
					memset(temp, 0, linesize);
					iter2=-1;
				}
				else if(section == 1)
				{
					printf("username = %s\n", temp);
					strcpy(current->user->name, temp);
					section++;
					memset(temp,0,linesize);
					iter2=-1;
				}
				else if(section == 2)
				{
					printf("password = %s\n", temp);
					strcpy(current->user->password, temp);
					section++;
					memset(temp,0,linesize);
					iter2=-1;
				}
			}
			iter++;
			iter2++;
		}
		free(line);
		free(temp);
		line = NULL;
		current->next = (struct userlinkedlist *)malloc(sizeof(struct userlinkedlist));
		memset ((void*)current->next, 0, sizeof(struct userlinkedlist));
		current = current->next;
	}
	current = list;
	while ((current != NULL) && (current->user != NULL))
	{
		printf("id = %d name = %s password = %s\n",current->user->userid, current->user->name, current->user->password);
		current = current->next;
	}
	fclose(fp);
	return EXIT_SUCCESS;
}



