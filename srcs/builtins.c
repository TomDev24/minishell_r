#include "minishell.h"

int	b_exit(char **argv)
{
	int	code;
	int chislo;

	code = 0;
	chislo = 0;
//	printf("builtin cmd: exit\n");
	if (argv[1])
		chislo = ft_atoi(argv[1]);
	//	if chislo is not number
	if (argv[1] && ft_strncmp(argv[1], "0", 2) != 0 && chislo == 0)
	{
		write(2, "sash: exit: ", 12);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
	else if (argv[1] && argv[2])
	{
		write(2, "sash: exit: too many arguments\n", 31);
		return (code);
	}
//	printf("chislo = %d\n", chislo);
	printf("exit\n");
	exit(chislo);
	return (code);
}

static void	err_msg(char *argv)
{
		write(2, "sash: cd: ", 10);
		write(2, argv, ft_strlen(argv));
		write(2, " not set\n", 9);
}

static void	chdir_check_error(char *argv)
{
//	int	i;

//	i = chdir(argv);
//	printf("i = %d\n", i);
//	if (i != 0)
	if (chdir(argv) != 0)
	{
		write(2, "sash: cd: ", 10);
//		printf("argv: %s\n", argv);
		write(2, argv, ft_strlen(argv));
		write(2, ": ", 2);
		perror("");
//		printf("Error: %s\n", strerror(errno));
	}
}

int	b_cd(char **argv)
{
	char	*vars[3];

	vars[0] = NULL;
	vars[0] = getcwd(vars[0], 1000);
//	printf("getcwd before action: %s\n", curr);
	if (argv[0] && argv[1] && argv[2])
		printf("sash: cd: too many arguments\n");
	else if (!argv[1] || (argv[1] && ft_strncmp(argv[1], "~", 2) == 0))
	{
		vars[2] = ht_get(mshell.hash_envp, "HOME");
		if (!vars[2])
			err_msg("HOME");
		else
			ht_set(mshell.hash_envp, "OLDPWD", vars[0]);
		chdir(vars[2]);
	}
	else if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
	{
		vars[1] = ht_get(mshell.hash_envp, "OLDPWD");
		if (!vars[1])
			err_msg("OLDPWD");
		else
			ht_set(mshell.hash_envp, "OLDPWD", vars[0]);
		chdir(vars[1]);
	}
	else
	{
		ht_set(mshell.hash_envp, "OLDPWD", vars[0]);
		chdir_check_error(argv[1]);
	}
	free(vars[0]);
//	printf("errno = %d\n", errno);
//	printf("exit_code = %d\n", errno/errno);
	return (0);
}

int	b_pwd(){
	char	buff[MAXPATHLEN];
	int	code;
	
	code = 0;
	if (getcwd(buff, MAXPATHLEN))
		printf("%s\n", buff);
	else
		code = 1;

	return (44 * (code +1));
}

int	b_echo(char	**argv){
	int	code;
	int	n_flag;

	code = 0;
	n_flag = 0;
	argv++;
	if (*argv && ft_strncmp(*argv, "-n", 3) == 0){
		n_flag = 1;
	while (ft_strncmp(*argv, "-n", 3) == 0)
		argv++;
	}
	while(*argv){
		ft_putstr_fd(*argv++, 1);
		if (*argv)
			ft_putstr_fd(" ", 1);
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (code);
}

int	b_env(){
	int 	code;
	int	i;

	code = 0;
	i = -1;
	char **s = hash_to_array(mshell.hash_envp);
	while(s[++i])
		printf("%s\n", s[i]);
	free_arr(s);
	return (code);
}

//works very BAD
//no free in while part
int	b_export(char **argv){
	int	code;
	char	**envp;
	char	**key_value;
	char	*equal_ptr;
	int	i;

	code = 0;
	i = -1;
	if (!argv[1]){
		//print sorted env
		envp = hash_to_array(mshell.hash_envp);
		envp = sort_array(envp);
		while(envp[++i])
			printf("declare -x %s\n", envp[i]);
		free_arr(envp);
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
	return (code);
}

int	b_unset(char **argv){
	int	code;

	code = 0;
	if (!argv[1])
		return code;
	while(*(++argv))
		ht_del(mshell.hash_envp, *argv);
	return (code);
}
