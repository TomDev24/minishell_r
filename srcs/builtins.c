#include "minishell.h"

int	b_pwd(){
	char	buff[MAXPATHLEN];
	int	code;
	
	code = 0;
	if (getcwd(buff, MAXPATHLEN))
		printf("my%s\n", buff);
	else
		code = 1;

	return code;
}

int	b_echo(char	**argv){
	int	code;
	int	n_flag;

	code = 0;
	n_flag = 0;
	argv++;
	printf("%s","myECHO");
	if (*argv && ft_strncmp(*argv, "-n", 2) == 0){
		n_flag = 1;
		argv++;
	}
	while(*argv){
		printf("%s",*argv++);
		if (*argv)
			printf(" ");
	}
	
	if (!n_flag)
		printf("\n");
	return code;
}
