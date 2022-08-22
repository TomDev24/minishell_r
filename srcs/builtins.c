#include "minishell.h"

int	b_pwd(){
	char	buff[MAXPATHLEN];
	int	code;
	
	code = 0;
	if (getcwd(buff, MAXPATHLEN))
		printf("%s\n", buff);
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
	if (*argv && ft_strncmp(*argv, "-n", 2) == 0){
		n_flag = 1;
		argv++;
	}
	while(*argv){
		ft_putstr_fd(*argv++, 1);
		//printf("%s",*argv++);
		if (*argv)
			ft_putstr_fd(" ", 1);
			//printf(" ");
	}
	
	if (!n_flag)
		ft_putstr_fd("\n", 1);
		//printf("\n");
	return code;
}

int	b_env(){
	int code;

	code = 0;
	char **s = hash_to_array(mshell.hash_envp);
	while(*s)
		printf("%s\n", *s++);
	return code;
}

//works very BAD
int	b_export(char **argv){
	int	code;
	char	**envp;
	char	**key_value;
	char	*equal_ptr;

	code = 0;
	if (!argv[1]){
		//print sorted env
		envp = hash_to_array(mshell.hash_envp);
		envp = sort_array(envp);
		while(*envp)
			printf("declare -x %s\n", *envp++);
	}
	//3 cases
	//1) var=value
	//2) var=
	//3) var
	while(*(++argv)){
		//printf("arg %s\n", *argv);
		key_value = ft_split(*argv, '=');	
		equal_ptr = ft_strchr(*argv, '=');
		if (key_value[1])
			ht_set(mshell.hash_envp, key_value[0], *argv); //key_value[0], key_value[1]); 
		else if (equal_ptr != NULL)
			ht_set(mshell.hash_envp, key_value[0], ft_strjoin(*argv,"empty")); 
		else
			ht_set(mshell.hash_envp, key_value[0], ""); 
		printf("value %s\n", ht_get(mshell.hash_envp, key_value[0]));
	}
	return code;
}

int	b_unset(char **argv){
	int	code;

	code = 0;
	if (!argv[1])
		return code;
	while(*(++argv))
		ht_del(mshell.hash_envp, *argv);
	return code;
}
