#include "minishell.h"

//HASH_ENVP should always have alocated key and str
void	init_hash_envp(char **envp){
	t_ht	*ht;
	char	*key;
	char	*tmp;

	mshell.hash_envp = ht_create(); 
	ht = mshell.hash_envp;
	while(*envp){
		//its very bad, think of something else
		key = *envp;
		tmp = ft_strchr(key, '=');
		*tmp = '\0';
		ht_set(ht, key, tmp + 1);
		*tmp = '=';
		envp++;
	}
}

