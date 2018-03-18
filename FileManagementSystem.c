#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Directory
{
	char filename[30];
	char filetype[6];

	struct Directory *link[100];
}dir;

void commands();
void folder(dir *, char []);
void display(dir *, int);
bool alreadyExists(dir *, char [], char []);
void showCurrentDirectory(dir *);
void changeCurrentDirectory(dir *);
void setRoot();

dir *root = NULL;
dir *currentDirectory;

int main()
{
	int i;

	dir *nf;
	nf = (dir*) malloc(sizeof(dir));

	strcpy(nf->filename , "root");
	strcpy(nf->filetype , "folder");

	for(i=0; i<100; i++)
	{
		nf->link[i] = NULL;
	}

	root = nf;
    currentDirectory = root;

    commands();

	char command[5];

	printf("Enter command:\n");
	gets(command);

	while(1)
	{
		if(strcmp(command, "mkdir") == 0)
			folder(currentDirectory, "folder");

		else if(strcmp(command, "file") == 0)
			folder(currentDirectory, "file");

		else if(strcmp(command, "ls") == 0)
			display(root, 0);

		else if(strcmp(command, "sd") == 0)
			showCurrentDirectory(currentDirectory);

		else if(strcmp(command, "cd") == 0)
			changeCurrentDirectory(currentDirectory);

		else if(strcmp(command, "root") == 0)
			setRoot();

		else if(strcmp(command, "help") == 0)
			commands();

		else if(strcmp(command, "exit") == 0)
			exit(0);

		else
			printf("Invalid command.\n");

		printf("\nEnter command:\n");
		gets(command);
	}

	return 0;
}

void commands()
{
	printf("mkdir: make a new folder\n");
	printf("file: create a new file\n");
	printf("ls: display the files and folders\n");
	printf("sd: show current directory\n");
	printf("cd: change current directory\n");
	printf("root: go back to root directory\n");
	printf("help: show the command list\n");
	printf("exit: close the program\n\n");
	printf("Commands are specific to this program and not to be confused with DOS/Linux terminal commands\n");
	printf("----represents a folder\n");
	printf("~~~~represents a file\n\n");
}

void folder(dir *temp, char filetype[6])
{
	int i = 0;

	for(i=0; i<100; i++)
	{
		if(temp->link[i] == NULL)
		{
			break;
		}
	}

	dir *nf;
	nf = (dir*) malloc(sizeof(dir));

	char foldername[30];

	printf("Enter the %s name:\n", filetype);
	gets(foldername);

	while(alreadyExists(temp, foldername, filetype))
	{
		printf("A %s with this name already exists.\n", filetype);

		printf("Enter the %s name again:\n", filetype);
		gets(foldername);
	}

	strcpy(nf->filetype, filetype);
	strcpy(nf->filename, foldername);

	temp->link[i] = nf;

	for(i=0; i<100; i++)
	{
		nf->link[i] = NULL;
	}

	printf("%s has been successfully created under %s\n", nf->filename, temp->filename);
}

void display(dir *temp, int level)
{
    if(temp == NULL)
        return;

	int i;

	if(strcmp(temp->filetype, "folder") == 0)
		for(i=1; i <= level; i++)
			printf("---");
	else
		for(i=1; i <= level; i++)
			printf("~~~");

	printf("%s\n", temp->filename);

	for(i=0; i<100 && temp->link[i] != NULL; i++)
	{
		display(temp->link[i], level+1);
	}
}

bool alreadyExists(dir *temp, char name[30], char type[6])
{
	int i;

	for(i=0; i<100 && temp->link[i] != NULL; i++)
	{
		if(strcmp(temp->link[i]->filetype, type) == 0)
			if(strcmp(temp->link[i]->filename, name) == 0)
				return true;
	}

	return false;
}

void showCurrentDirectory(dir *cur)
{
    printf("The current directory is: %s\n", cur->filename);
}

void changeCurrentDirectory(dir *temp)
{
	char foldername[30];

	printf("Enter the directory the you want to open: ");
	gets(foldername);

	int i;
	bool found = false;
	for(i=0; i<100 && temp->link[i] != NULL; i++)
	{
		if(strcmp(temp->link[i]->filetype, "folder") == 0)
			if(strcmp(temp->link[i]->filename, foldername) == 0)
			{
				found = true;

				currentDirectory = temp->link[i];
			}
	}

	if(found == false)
	{
		printf("Error. Directory not found.\n");
	}
}

void setRoot()
{
	currentDirectory = root;
}
