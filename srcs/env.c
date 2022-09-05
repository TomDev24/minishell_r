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

void	change_question(t_token *tokens, t_cmd *cmd){
	//char	*s;

	//s = ft_strchr(tokens->value, '$');	
	//if (!s)
	//	return;
	//printf("change\n");
	if (ft_strncmp(tokens->value, "$?", 3) == 0){
		printf("CHAING DSK %s#\n", tokens->value);
		free(tokens->value);
		tokens->value = ft_itoa(mshell.exit_code);
		printf("%s\n", tokens->value);
	}	
}

void	update_mshell(int code, int cmd_i, t_cmd *cmd){
	t_token	*tokens;

	mshell.exit_code = code;
	tokens = mshell.tokens;
	//printf("UPDAT# MSHEL cmd-i %d\n", cmd_i);
	while (tokens){
		//printf("tkns-i %d\n", tokens->i);
		if (tokens->i == cmd_i + 1)
			change_question(tokens, cmd);
		else if (tokens->i > cmd_i + 1)
			break;
		tokens = tokens->next;
	}
}
