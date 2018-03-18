#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>	//for bool type variables

typedef struct Directory
{
	char filename[30];	//name of the current file or folder
	char filetype[10];	//type of file, indicating a "file" or a "folder" 

	struct Directory *link[100];	//an array of pointers of type Directory to store upto 100 subdirectories/files
}dir;

void commands();								//displays all commands
void extractInput(char *, char *, char *);		//extracts the command and the filename from input
void folder(dir *, char [], char []);			//creates new file/folder in the current directory 
void display(dir *, int);						//displays all the files and folders recursively
void ls(dir *);									//displays the contents of current directory
bool alreadyExists(dir *, char [], char []);	//checks if a given file/folder already exists in the current directory
void showCurrentDirectory(dir *);				//shows the name of thecurrent directory
void changeCurrentDirectory(dir *, char []);	//changes the working directory
void setRoot();									//sets current directory to the root folder

dir *root = NULL;		//the root directory
dir *currentDirectory;	//the current directory

int main()
{
	int i;

	dir *newFolder;
	newFolder = (dir*) malloc(sizeof(dir));

	strcpy(newFolder->filename , "root");	//name the root folder as "root"
	strcpy(newFolder->filetype , "folder");	//set root as a "folder"

	//set all links to NULL
	for(i=0; i<100; i++)
	{
		newFolder->link[i] = NULL;
	}

	root = newFolder;
    currentDirectory = root;

    char input[40];		//input variable
	char command[5];	//extracted command
	char filename[30];	//extracted filename

	printf("FILE MANAGEMENT SYSTEM\n");
	printf("enter \"help\" for help\n\n\n");

	gets(input);	//take input
	extractInput(input, command, filename);		//extract the command and filename from input

	while(1)	//infinite loop, works until the user wants to exit
	{
		if(strcmp(command, "mkdir") == 0)
			folder(currentDirectory, filename, "folder");

		else if(strcmp(command, "file") == 0)
			folder(currentDirectory, filename, "file");

		else if(strcmp(command, "tree") == 0)
			display(root, 0);

		else if(strcmp(command, "ls") == 0)
			ls(currentDirectory);

		else if(strcmp(command, "sd") == 0)
			showCurrentDirectory(currentDirectory);

		else if(strcmp(command, "cd") == 0)
			changeCurrentDirectory(currentDirectory, filename);

		else if(strcmp(command, "help") == 0)
			commands();

		else if(strcmp(command, "exit") == 0)
			exit(0);

		else
			printf("Invalid command.\n");

		gets(input);
		extractInput(input, command, filename);
	}

	return 0;
}

void extractInput(char *input, char *command, char *filename)
{
	int i, j;	//loop variables

	for(i=0; input[i] != ' ' && input[i] != '\0'; i++)
	{
		command[i] = input[i];
	}

	command[i] = '\0';

	i++;

	for(j=0; input[i] != '\0'; i++, j++)
	{
		filename[j] = input[i];
	}

	filename[j] = '\0';
}

void commands()
{
	printf("mkdir <foldername>: make a new folder\n");
	printf("file <filename>: create a new file\n");
	printf("tree: display all the files and folders\n");
	printf("ls: display the files and folders in current directory\n");
	printf("sd: show current directory\n");
	printf("cd <foldername>: change current directory\n");
	printf("cd root: go back to root directory\n");
	printf("help: show the command list\n");
	printf("exit: close the program\n\n");
	printf("Commands are specific to this program and not to be confused with DOS/Linux terminal commands\n");
	printf("----represents a folder\n");
	printf("~~~~represents a file\n\n");
}

void folder(dir *temp, char filename[30], char filetype[10])
{
	//check for duplicate filename
	if(alreadyExists(temp, filename, filetype))
	{
		printf("A %s with this name already exists.\n", filetype);

		return;
	}

	int i = 0;

	//loop to enter the first link which is set to NULL
	for(i=0; i<100; i++)
	{
		if(temp->link[i] == NULL)
		{
			break;
		}
	}

	dir *nf;	//new file/folder
	nf = (dir*) malloc(sizeof(dir));

	temp->link[i] = nf;	//link the new folder to ith position of temp's link

	strcpy(nf->filetype, filetype);	//copy the filetype to nf
	strcpy(nf->filename, filename);	//copy the filename to nf

	//set new file/folders links to NULL
	for(i=0; i<100; i++)
	{
		nf->link[i] = NULL;
	}

	printf("%s has been successfully created under %s\n", nf->filename, temp->filename);
}

void ls(dir *temp)
{
	int i;	//loop variable
	for(i=0; i<100 && temp->link[i] != NULL; i++)
	{
		printf("%s\t", temp->link[i]->filename);
	}
	printf("\n");
}

void display(dir *temp, int level)
{
    if(temp == NULL)
        return;

	int i;	//loop variable

	//print '-' for folders
	if(strcmp(temp->filetype, "folder") == 0)
		for(i=1; i <= level; i++)
			printf("---");

	//print '~' for files
	else if(strcmp(temp->filetype, "file") == 0)
		for(i=1; i <= level; i++)
			printf("~~~");

	printf("%s\n", temp->filename);

	//call all the subdirectories/files recursively
	for(i=0; i<100 && temp->link[i] != NULL; i++)
	{
		display(temp->link[i], level+1);	//increment level to print more dashes
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

void changeCurrentDirectory(dir *temp, char foldername[30])
{
	//to set current directory to "root"
	if(strcmp(foldername, "root") == 0)
	{
		setRoot();
		showCurrentDirectory(currentDirectory);
		return;
	}

	int i; //loop variable
	bool found = false;
	for(i=0; i<100 && temp->link[i] != NULL; i++)
	{
		if(strcmp(temp->link[i]->filetype, "folder") == 0)
			if(strcmp(temp->link[i]->filename, foldername) == 0)
			{
				found = true;

				currentDirectory = temp->link[i];

				break;
			}
	}

	//if directory does not exist, create one and set current directory to it
	if(found == false)
	{
		printf("Directory not found. Creating new directory.\n");
		folder(currentDirectory, foldername, "folder");
		changeCurrentDirectory(currentDirectory, foldername);
		return;
	}

	showCurrentDirectory(currentDirectory);
}

void setRoot()
{
	currentDirectory = root;
}
