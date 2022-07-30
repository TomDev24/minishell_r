#include "minishell.h"

void	init_hash_envp(char **envp){
	t_ht	*ht;
	char	*key;
	char	*tmp;

	ht = mshell.hash_envp;
	while(*envp){
		//its very bad, think of something else
		key = ft_strdup(*envp);
		tmp = ft_strchr(key, '=');
		*tmp = '\0';
		ht_set(ht, key, *envp);
		envp++;
	}

	//print_ht(ht);
	//char **s = hash_to_array(ht);
	//while(*s)
	//	printf("%s\n", *s++);
}



