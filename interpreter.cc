#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "command.h"

#define MAX_USR_LEN 128

using std::string;
using std::cin;
using std::cout;
using std::ifstream;

char user_name[MAX_USR_LEN];
char domain_name[MAX_USR_LEN];
const char* parameter[10];

int main(int argc, char** argv)
{	
	init_shell();
	init_map();
	clear_pipe();
	
	chdir(getenv("HOME"));
	setenv("PWD", getenv("HOME"), 1);

	while (!halt) 
	{
		cuserid(user_name);
		gethostname(domain_name, MAX_USR_LEN);
		getcwd(working_directory, MAX_PATH_LEN);
		char* pos = strstr(working_directory, getenv("HOME"));
		if (pos)
		{
			char path_buf[MAX_PATH_LEN] = "~";
			strcat(path_buf, pos + strlen(getenv("HOME")));
			strcpy(working_directory, path_buf);
		} 
		// dup2(f_stdin, 0);
		if (!argv[1])
		{
			printf("\033[36m%s@%s\033[0m", user_name, domain_name);
			printf(":");
			printf("\033[35m%s\033[0m", working_directory);
			printf("$ ");
			parameter[0] = "myshell";
			for (int i = 1; i < 10; i++)
			{
				parameter[i] = NULL;
			}
			string command_in;
			getline(cin, command_in);
			c_interpret(command_in);

		}
		else
		{
			ifstream command_file(argv[1]);
			if (command_file.fail())
			{
				printf("%s: No such file or directory\n", argv[1]);
				exit(0);
			}
			for (int i = 0; i < 10; i++)
			{
				if (i + 1 < argc)
				{
					parameter[i] = argv[i + 1];
				}
				else
				{
					parameter[i] = NULL;
				}
			}
			string command_in;
			while (!command_file.eof())
			{
				getline(command_file, command_in);
				c_interpret(command_in);
			}
			exit(0);
		}
		
	}
	return 0;
}