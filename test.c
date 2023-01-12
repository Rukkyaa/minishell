#include <stdio.h>
#include <unistd.h>

int main(int ac, char *av[])
{
	printf("Hello World\n");
	if (ac == 2)
	{
		    char s[100];
  
    	// printing current working directory
    	printf("%s\n", getcwd(s, 100));
  
    	// using the command
    	chdir("../..");
  
    	// printing current working directory
    	printf("%s\n", getcwd(s, 100));
  
    	// after chdir is executed
    	return 0;
	}
}