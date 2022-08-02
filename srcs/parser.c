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


/*
t_token		*look_next_quote(t_list *q_list, t_token *tokens, int qts_type){
	t_token *prev;

	prev = tokens;	
	while(tokens && (tokens->type == Q1 || tokens->type == Q2) && tokens->type != qts_type ){

		ft_lstadd_back(&q_list, ft_lstnew(tokens));

		prev = tokens;	
		tokens = tokens->next;
	}
	
	return prev;
} */


void	handle_quote_block(t_cmd *new, t_quotes *quotes, t_list *last){
	t_list				*head;
	char				*value;
	int		str_len;
	t_token		*t;
	t_token		*last_p;
	t_token		*first_p;
	char		*tmp;
	int		i;
	
	i = 0;
	head = quotes->q_list;
	last_p =last->content;
	first_p = head->content;
	str_len = last_p->addr - first_p->addr;
	//printf("STR_LEN %d\n", str_len);
	value = (char*)ft_calloc(str_len, sizeof(char));
	if (!value){
		printf("Cant allocate line\n");
		exit(1);
	}	

	tmp = first_p->addr;
	while(tmp + i != last_p->addr){
		//printf("%c\n", value[i]);
		value[i] = tmp[i];	
		i++;
	}	
	//
	//char	*r;
	//r = list_to_line(head);
	printf("QUOTES BLOCK: %s\n", value);
	//exit(2);
	//
	if (!value)
		exit(1);
	t = (t_token*)malloc(sizeof(t_token));
	if (!t)
		exit(1); // make better error	
	t->type = ARG;
	t->value = value; //list_to_line(q_list);
	t->addr= NULL; 
	t->next = NULL;
	ft_lstadd_back(&new->args, ft_lstnew(t));
	//reset quote block
	quotes->q_list = NULL;
}

t_token	*pack_cmd(t_token *tokens, t_cmd **cmds){
	t_cmd		*new;
	t_quotes	*quotes;
	t_list		*el;
	new = (t_cmd*)malloc(sizeof(t_cmd));
	quotes = init_quotes();
	if (!new)
		exit(1); // make better error
	init_cmd(new);
	while(tokens && tokens->type != PIPE){
		//handling quotes
		if (quotes->type != 0){
			el = ft_lstnew(tokens);
			ft_lstadd_back(&quotes->q_list, el);
		}
		/////////////////
		else if (tokens->type == CMD && !new->cmd)
			new->cmd = tokens;
		else if (tokens->type == ARG)
			ft_lstadd_back(&new->args, ft_lstnew(tokens));
		else if (tokens->type == IN){
			if (tokens->next->type == FILEN){
				tokens = tokens->next;
				new->infile = tokens->value;
			}
		}	
		else if (tokens->type == OUT){
			if (tokens->next->type == FILEN){
				tokens = tokens->next;
				new->outfile = tokens->value;
			}
		}
		if (tokens->type == Q1 || tokens->type == Q2){
			//quotes struct, started quote

			if (quotes->type == 0){
				el = ft_lstnew(tokens);
				ft_lstadd_back(&quotes->q_list, el);
				quotes->type = tokens->type;
			}
			else if (quotes->type == tokens->type){
				quotes->type = 0;
				handle_quote_block(new, quotes, el);
			}

			////PLACE HERE
			
		}
		tokens = tokens->next;
	}	
	//when packing in **argv everything should be trimmed
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




////PLACE HERE
/*
			if (quotes->type && tokens->type != quotes->type){
				t_list *q_list;
				q_list = ft_lstnew(tokens);
				tokens = look_next_quote(q_list, tokens->next, quotes->type);

				t_token *t;
				t = (t_token*)malloc(sizeof(t_token));
				if (!t)
					exit(1); // make better error	
				t->type = ARG;
				t->value = list_to_line(q_list);
				t->addr= NULL; 
				t->next = NULL;
				ft_lstadd_back(&new->args, ft_lstnew(t));
			}
*/



