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

	return res;
}

static	char *list_to_line(t_list *lst){
	char	*res;
	t_token	*el;

	res = "";
	while(lst){
		el = lst->content;
		res = ft_strjoin(res, el->value);//ft_strjoin(el->value, " "));
		lst = lst->next;
	}
	return res;
}

t_token		*look_next_quote(t_list *q_list, t_token *tokens, int qts_type){
	t_token *prev;

	
	while(tokens && (tokens->type == Q1 || tokens->type == Q2) && tokens->type != qts_type ){

		ft_lstadd_back(&q_list, ft_lstnew(tokens));

		prev = tokens;	
		tokens = tokens->next;
	}
	
	return prev;
}

t_token	*pack_cmd(t_token *tokens, t_cmd **cmds){
	t_cmd		*new;
	t_quotes	*quotes;

	new = (t_cmd*)malloc(sizeof(t_cmd));
	quotes = init_quotes();
	if (!new)
		exit(1); // make better error
	init_cmd(new);
	while(tokens && tokens->type != PIPE){
		if (tokens->type == CMD && !new->cmd)
			new->cmd = tokens;
		if (tokens->type == ARG)
			ft_lstadd_back(&new->args, ft_lstnew(tokens));
		if (tokens->type == IN){
			if (tokens->next->type == FILEN){
				tokens = tokens->next;
				new->infile = tokens->value;
			}
		}	
		if (tokens->type == OUT){
			if (tokens->next->type == FILEN){
				tokens = tokens->next;
				new->outfile = tokens->value;
			}
		}
		if (tokens->type == Q1 || tokens->type == Q2){
			//quotes struct, started quote
			if (quotes->type == 0)
				quotes->type = tokens->type;
			else if (quotes->type == tokens->type)
				quotes->type = 0;

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
