#include "minishell.h"

void	init_cmd(t_cmd *cmd){
	cmd->i = 0;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->next = NULL;
}

t_cmd	*allocate_cmd(){
	t_cmd	*new;

	new = (t_cmd*)malloc(sizeof(t_cmd));
	if (!new)
		exit(1); // make better error init_cmd(new);
	init_cmd(new);
	return new;
}

void	add_cmd_to_list(t_cmd **cmds, t_cmd *new){
	t_cmd 	*tmp;
	int	i;
	
	i = 1;
	tmp = *cmds;
	new->i = 0;
	if (*cmds == NULL){
		*cmds = new;
		return;
	}
	while (tmp->next && ++i)
		tmp = tmp->next;
	new->i = i;
	tmp->next = new;
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
		//if (!tkn->value){ printf("null\n"); tmp = tmp->next; continue;}
		//printf("tkn->value %s\n", tkn->value);
		//printf("tkn->value[0] %c\n", tkn->value[0]);
		//QUOTES LOGIC !!!!!! GET IT OUT
		if (tkn->value[0] == 0 && tkn->addr[0] != '"' && tkn->addr[0] != '\'') //val is null and not " or '
			size--; // correcting size for valid return
		else
			*(res++) = tkn->value;
		tmp = tmp->next;
	}
	*res = NULL;
	return res - size;
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

//we got problems with redirection
t_token	*pack_cmd(t_token *tokens, t_cmd **cmds){
	t_cmd		*new;

	new = allocate_cmd();
	while(tokens && tokens->type != PIPE){
		if (tokens->type == CMD && !new->cmd)
			new->cmd = tokens;
		else if (tokens->type == ARG)
			ft_lstadd_back(&new->args, ft_lstnew(tokens));
		else if (tokens->type == IN)
			tokens = save_redirection(tokens, new, IN);
		else if (tokens->type == OUT)
			tokens = save_redirection(tokens, new, OUT);
		tokens = tokens->next;
	}

	new->argv = make_argv(new);
	add_cmd_to_list(cmds, new);
	return tokens;
}

t_cmd	*parser(t_token **tokens){
	t_cmd		*cmds;
	
	cmds = NULL;

	pretty_lexer(*tokens);
	unquote(tokens);
	printf("\nAFTER:\n");
	pretty_lexer(*tokens);
	exit(1);

	//We should read each token until PIPE or end_of_list (; is not included for now)	
	while(*tokens){
		*tokens = pack_cmd(*tokens, &cmds);
		if (*tokens && (*tokens)->type == PIPE)
			*tokens = (*tokens)->next;
	}
	return cmds;
}
