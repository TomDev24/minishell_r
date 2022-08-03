#include "minishell.h"

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
	//raise(SIGTRAP);
	tmp = cmd->args;

	res = (char **)malloc(sizeof(res) * (size + 1));
	if (!res)
		exit(1); // make better exit

	*(res++) = cmd->cmd->value;
	while (tmp){
		tkn = tmp->content;
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

static	char *list_to_line(t_list *lst){
	char	*res;
	t_token	*el;
	int	i;

	res = "";
	i = 0;
	while(lst){
		el = lst->content;
		printf("i: %d, value: %s\n", i, el->value);
		res = ft_strjoin(res, el->value); //ft_strjoin(el->value, " "));
		lst = lst->next;
		i++;
	}
	return res;
}


t_token	*find_token_by_addr(char *addr, t_token *tokens){
	t_token *res;
	t_token *prev;
	
	res = NULL;
	prev = NULL;
	//printf("ADDR%s\n", addr);
	while(tokens){
		//printf("TOKEN ADDR %s\n", tokens->addr);
		if (ft_strncmp(tokens->addr, addr, ft_strlen(addr)) == 0){ //(tokens->addr == addr)
			return prev;
		}
		prev = tokens;
		tokens = tokens->next;
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

t_token	*handle_quote_block(t_cmd *new, t_quotes *quotes, t_list *last, t_token *tokens){
	char		*value;
	t_token		*last_p;
	t_token		*first_p;
	char		*tmp;
	int		i;
	
	i = 0;
	last_p = last->content;
	first_p = quotes->q_list->content;

	//if something is next to quote, it must be part of quote_block
	while (*(last_p->addr + 1) && *(last_p->addr + 1) != ' ')
		last_p->addr++;

	value = (char*)ft_calloc(last_p->addr - first_p->addr, sizeof(char));
	if (!value)
		exit(1);
	tmp = first_p->addr + 1; //skip first quote
	while(tmp + i < last_p->addr)
		if(tmp[i] != *(first_p->value))
			value[i++] = tmp[i];
		else
			tmp++;

	add_token_to_args(new, value);
	quotes->q_list = NULL;
	//finding next token, after adding a lot of them to q_list
	while (*(last_p->addr + 1) && *(last_p->addr + 1) == ' ')
		last_p->addr++;
	return find_token_by_addr(last_p->addr + 1, tokens);
}

t_cmd	*allocate_cmd(){
	t_cmd	*new;

	new = (t_cmd*)malloc(sizeof(t_cmd));
	if (!new)
		exit(1); // make better error init_cmd(new);
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
