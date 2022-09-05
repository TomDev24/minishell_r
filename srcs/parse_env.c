#include "minishell.h"

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
	//t_token	*prev;
	char	*tmp;
	t_token	*tmp_tkn;

	first = current;
	//prev = NULL;
	while (current && *current->addr == '$'){
		if (first->end_addr && first->end_addr != current->addr)
			break;

		first->end_addr = tkn_eof(current) + 1; //tkn_eof points on last valid char
		key = current->value + 1;
		value = ht_get(mshell.hash_envp, key);
		if (*key == 0)
			value = "$";
		free(current->value);
		current->value = "";
		if (value){
			if (context->q_type != -1)
				context->evars_len += ft_strlen(value);
			tmp = first->value;
			first->value = ft_strjoin(first->value, value);
			if (*tmp)
				free(tmp);
		}
		//prev = current;
		tmp_tkn = current;
		current = current->next;
		if (first != tmp_tkn)
			free_tkn(&tmp_tkn);
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