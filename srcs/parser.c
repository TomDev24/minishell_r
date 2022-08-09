#include "minishell.h"

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

void	add_cmd_to_list(t_cmd **cmds, t_cmd *new){
	t_cmd 	*tmp;
	int	i;
	
	i = 1;
	tmp = *cmds;
	if (*cmds == NULL){
		new->i = 0;
		*cmds = new;
		return;
	}
	while (tmp->next){	
		i++;
		tmp = tmp->next;
	}
	new->i = i;
	tmp->next = new;
}

void	init_cmd(t_cmd *cmd){
	cmd->i = 0;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->next = NULL;
}

char	**make_argv(t_cmd *cmd){
	//when packing in **argv everything should be trimmed
	char	**res;
	int	size;
	t_list	*tmp;
	t_token	*tkn;

	size = 1 + ft_lstsize(cmd->args);
	tmp = cmd->args;

	res = (char **)malloc(sizeof(res) * (size + 1));
	if (!res)
		exit(1); // make better exit

	*(res++) = cmd->cmd->value;
	while (tmp){
		tkn = tmp->content;
		/*
		printf("tkn->value %s\n", tkn->value);
		if (!tkn->value){
			printf("null\n");
			tmp = tmp->next;
			continue;
		}*/
		//if (tkn->value)
		*(res++) = tkn->value;
		tmp = tmp->next;
	}
	*res = NULL;
	return res - size;
}

t_quotes	*init_quotes(){
	t_quotes	*res;
	
	res = malloc(sizeof(t_quotes));
	res->type = 0;
	res->q_list = NULL;

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

void	add_token_to_args(t_cmd *new, char *value){
	t_token		*t;

	if (!value) //should be diffrent check
		exit(1);
	t = (t_token*)malloc(sizeof(t_token));
	if (!t)
		exit(1); // make better error	
	t->type = ARG;
	t->value = value; //list_to_line(q_list);
	t->addr= NULL; 
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
			} 
		}
		if(tmp[i] != *(first_p->value)){
			value[i] = tmp[i];
			i++;
		}
		else
			tmp++;
	}
	add_token_to_args(new, value);
	quotes->q_list = NULL;
	//finding next token, after adding a lot of them to q_list
	while (*(last_p->addr + 1) && *(last_p->addr + 1) == ' ')
		last_p->addr++;
	//!!! HERE MIGHT BE ERROR, BECAUSE TOKENS POINTS TO LAST ITEM
	//OR ITS EVEN NOT MUST TO USE THIS FUNC HERE
	return find_token_by_addr(last_p->addr + 1, tokens);
}

t_cmd	*allocate_cmd(){
	t_cmd	*new;

	new = (t_cmd*)malloc(sizeof(t_cmd));
	if (!new)
		exit(1); // make better error init_cmd(new);
	init_cmd(new);
	return new;
}

t_token	*save_redirection(t_token *tokens, t_cmd *new, int FD_TYPE){
	if (tokens->next->type == FILEN){
		tokens = tokens->next;
		if (FD_TYPE == IN) 
			new->infile = tokens->value;
		else if (FD_TYPE == OUT)
			new->outfile = tokens->value;
	}
	
	return tokens;
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

//we got problems with redirection
t_token	*pack_cmd(t_token *tokens, t_cmd **cmds){
	t_cmd		*new;
	t_quotes	*quotes;
	t_list		*el;

	el = NULL;
	new = allocate_cmd();
	quotes = init_quotes();
	while(tokens && (tokens->type != PIPE || quotes->type != 0)){
		if (*(tokens->addr) == '$')
			tokens->value = expand(tokens, tokens->addr + 1);
		if (quotes->type != 0){
			el = ft_lstnew(tokens);
			ft_lstadd_back(&quotes->q_list, el);
		}
		else if (tokens->type == CMD && !new->cmd)
			new->cmd = tokens;
		else if (tokens->type == ARG)
			ft_lstadd_back(&new->args, ft_lstnew(tokens));
		else if (tokens->type == IN)
			tokens = save_redirection(tokens, new, IN);
		else if (tokens->type == OUT)
			tokens = save_redirection(tokens, new, OUT);
		if (tokens->type == Q1 || tokens->type == Q2)
			tokens = quotes_manager(tokens, quotes, el, new);
		if (!tokens)
			break;
		tokens = tokens->next;
	}

	new->argv = make_argv(new);
	add_cmd_to_list(cmds, new);
	return tokens;
}

t_cmd	*parser(t_token *tokens){
	t_cmd		*cmds;
	
	cmds = NULL;	
	//We should read each token until PIPE or end_of_list (; is not included for now)	
	while(tokens){
		tokens = pack_cmd(tokens, &cmds);
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return cmds;
}
