#include "minishell.h"

int	cmd_push(t_cmd **cmds, int i, char **argv){
	t_cmd	*new;
	t_cmd *tmp;

	//printf("CReating token\n");
	tmp = *cmds;
	new = (t_cmd*)malloc(sizeof(t_cmd));
	if (!new)
		exit(1); // make better error
	
	new->i = i;
	new->argv = argv;
	new->next = NULL;
	if (*cmds == NULL){
		*cmds = new;
		return 1;
	}
	while (tmp->next){	
		tmp = tmp->next;
	}
	tmp->next = new;	
	return 1;
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

t_token	*pack_cmd(t_token *tokens, t_cmd **cmds){
	//cmd_push(cmds, 0, ft_split("ls -la", ' ')); 
	t_cmd	*new;

	new = (t_cmd*)malloc(sizeof(t_cmd));
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
