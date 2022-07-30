#include "minishell.h"

void	init_hash_envp(char **envp){
	t_ht	*ht;
	char	*key;
	char	*tmp;

	shell.hash_envp = ht_create(); 
	ht = shell.hash_envp;
	while(*envp){
		//its very bad, think of something else
		key = ft_strdup(*envp);
		tmp = ft_strchr(key, '=');
		*tmp = '\0';
		ht_set(ht, key, *envp);
		envp++;
	}

	print_ht(ht);
}
