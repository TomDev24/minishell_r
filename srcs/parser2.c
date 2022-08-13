#include "minishell.h"

t_quotes	*init_quotes(){
	t_quotes	*res;
	
	res = malloc(sizeof(t_quotes));
	res->type = 0;
	res->q_list = NULL;

	return res;
}

//test it
char	*expand(t_token *tokens, char *value){
	char	*line;
	char	*pos;
	char	*res;

	res = "";
	line = tokens->value + 1;
	if (*value == 0 || *value == ' ' || *value == '$')
		return tokens->value;

	while(*line){
		pos = ft_strchr(line, '$');	
		if (pos)
			*pos = 0;
		else
			pos = ft_strrchr(line, 0) - 1;
		res = ft_strjoin(res, ht_get(mshell.hash_envp, line)); 
		line = pos + 1;
	}
	if (res == NULL)
		res = "";
	return res;
}


//Needs refactor // I have some questions about it
t_token	*find_token_by_addr(char *addr, t_token *tokens){
	t_token *res;
	t_token *prev;
	
	res = NULL;
	prev = NULL;
	//printf("ADDR%s\n", addr);
	while(tokens){
		//printf("TOKEN ADDR %s\n", tokens->addr);
		//if (ft_strncmp(tokens->addr, addr, ft_strlen(addr)) == 0){ //(tokens->addr == addr)
		if (tokens->addr == addr){
			return prev;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return res;
}

//REFACTOR NEEDED
t_token	*find_item_by_addr(char *addr, t_list *q_list){
	t_token *res;
	t_token *cur;
	
	res = NULL;
	while(q_list){
		cur = q_list->content;
		if (cur->addr == addr)
			return cur;
		q_list = q_list->next;
	}
	return res;
}

void	add_token_to_args(t_cmd *new, char *value, char *quote){
	t_token		*t;

	if (!value) //should be diffrent check
		exit(1);
	t = (t_token*)malloc(sizeof(t_token));
	if (!t)
		exit(1); // make better error	
	//its not just arg, i should add logic to choose corect one
	t->type = ARG;
	t->value = value; //list_to_line(q_list);
	t->addr= quote; 
	t->next = NULL;
	ft_lstadd_back(&new->args, ft_lstnew(t));

}

int	count_vars_len(t_list *q_list){
	int	res;
	t_token	*cur;

	res = 0;
	while(q_list){
		cur = q_list->content;
		if (*(cur->addr) == '$' && cur->value){
			res += ft_strlen(cur->value);
		}
		q_list = q_list->next;
	}

	return res;
}

t_token	*handle_quote_block(t_cmd *new, t_quotes *quotes, t_list *last, t_token *tokens){
	char		*value;
	t_token		*last_p;
	t_token		*first_p;
	char		*tmp;
	int		i;
	t_token		*var_token;
	
	i = 0;
	last_p = last->content;
	first_p = quotes->q_list->content;

	//if something is next to quote, it must be part of quote_block
	while (*(last_p->addr + 1) && *(last_p->addr + 1) != ' ')
		last_p->addr++;
	if (*(last_p->addr + 1) == 0 && *(last_p->addr) != ' ')  
		last_p->addr++;
	
	//Better memory managment??
	value = (char*)ft_calloc((last_p->addr - first_p->addr) + count_vars_len(quotes->q_list), sizeof(char));
	if (!value)
		exit(1);
	tmp = first_p->addr + 1; //skip first quote
	while(tmp + i < last_p->addr){
		if (tmp[i] == '$'){
			var_token = find_item_by_addr(tmp + i, quotes->q_list);
			//raise(SIGTRAP);
			if (var_token && var_token->value[0] != 0){
				ft_strlcpy(value + i, var_token->value, ft_strlen(var_token->value) + 1);
				i += ft_strlen(var_token->value) + 1;
			} else if (var_token->value[0] == 0){
				//handling one $ or not existing var
				if (tmp[i + 1] == ' ' || tmp[i + 1] == 0 || tmp[i+1] == *(first_p->value))
					value[i++] = '$';
				else{
					tmp++; //skip $
					while (tmp[i] && tmp[i] != ' ')
						tmp++;
				}
			}
		}
		if(tmp[i] != *(first_p->value)){
			value[i] = tmp[i];
			i++;
		}
		else
			tmp++;
	}
	//printf("Value %s||\n", value);
	add_token_to_args(new, value, first_p->value);
	quotes->q_list = NULL;
	//finding next token, after adding a lot of them to q_list
	while (*(last_p->addr + 1) && *(last_p->addr + 1) == ' ')
		last_p->addr++;
	//!!! HERE MIGHT BE ERROR, BECAUSE TOKENS POINTS TO LAST ITEM
	//OR ITS EVEN NOT MUST TO USE THIS FUNC HERE
	return find_token_by_addr(last_p->addr + 1, tokens);
}

t_token *quotes_manager(t_token *tokens, t_quotes *quotes, t_list *el, t_cmd *new){
	if (quotes->type == 0){
		el = ft_lstnew(tokens);
		ft_lstadd_back(&quotes->q_list, el);
		quotes->type = tokens->type;
	}
	else if (quotes->type == tokens->type){
		quotes->type = 0;
		tokens = handle_quote_block(new, quotes, el, tokens);
	}
	return tokens;
}

