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


char	*try_replace_env(t_token *tokens, char *st_addr, char *value, int *i){
	t_token	*evar;
	int	j;

	//token of $ should be 100%
	j = 0;
	evar = get_token_by_addr(st_addr, tokens, 0);
	if (*evar->value){
		while (evar->value[j]){
			value[*i] = evar->value[j++];
			*i = *i + 1;
		}
	}
	return evar->end_addr ? evar->end_addr : ++st_addr;
}

void	change_token_value(t_token *current, t_stack *context){
	char	*value;
	char	*key;
	t_token	*first;
	t_token	*prev;

	first = current;
	prev = NULL;
	while (current && *current->addr == '$'){
		if (prev && first->end_addr != current->addr)
			break;
		//else
		
		first->end_addr = tkn_eof(current) + 1; //tkn_eof points on last valid char
		key = current->value + 1;
		value = ht_get(mshell.hash_envp, key);
		if (*key == 0)
			value = "$";
		current->value = "";
		if (value){
			if (context->q_type != -1)
				context->evars_len += ft_strlen(value);
			first->value = ft_strjoin(first->value, value);
		}

		prev = current;
		current = current->next;
	}
	first->next = current;
}


void	manage_evar(t_token *current, t_stack *context){
	if(current->type != CMD && current->type != ARG
		&& current->type != FILEN && current->type != DELIMITER)
		return;
	if(*current->addr != '$')
		return;	
	if (ft_strncmp(current->value, "$", 2) == 0)
		return;
	
	//printf("current->value %s\n", current->value + 1);
	change_token_value(current, context);
}
