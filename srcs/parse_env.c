#include "minishell.h"

/*
void	change_token_value(t_token *current, char *key){
	char	*value;
	char	*addr;

	//printf("--RECUR COUNT\n");
	//if(key)
	//	printf("key %s\n", key+1);

	if (!key)
		return;
	key++;
	addr = ft_strchr(key, '$');
	if (addr)
		*addr = 0;
	value = ht_get(mshell.hash_envp, key);
	if (value){
		if (*(key-1) == 0){
			current->value = ft_strjoin(current->value, value);
		}
		else{
			current->value =value;
		}
	}
	else{
		if (*(key-1) != 0)
			current->value = "";
	}
	change_token_value(current, addr);
}
*/


void	try_replace_env(t_token *tokens, t_token *st_token, char *value, int *i){
	t_token	*evar;
	t_token *evar_next;

	if (*st_token->addr != '$')
		return;

	//token of $ should be 100%
	evar = get_token_by_addr(st_token->addr, tokens, 0);
	evar_next = evar->next;
	if (evar_next)
		st_token->addr = evar_next->addr;
	if (*evar->value){
		while (*evar->value)
			value[*i++] = *evar->value++;
	}
}

void	change_token_value(t_token *current, t_stack *context){
	char	*value;
	char	*key;

	key = current->value + 1;
	value = ht_get(mshell.hash_envp, key);
	if (value){
		if (context->q_type != -1)
			context->evars_len += ft_strlen(value);
		current->value = value;	
	}
	else
		current->value = "";
}


void	manage_evar(t_token *current, t_stack *context){
	if(current->type != CMD && current->type != ARG)
		return;
	if(*current->addr != '$')
		return;	
	if (ft_strncmp(current->value, "$", 2) == 0)
		return;
	
	//printf("current->value %s\n", current->value + 1);
	change_token_value(current, context);
}
