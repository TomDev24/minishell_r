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

t_token	*pack_cmd(t_token *tokens, t_cmd **cmds){
	cmd_push(cmds, 0, ft_split("ls -la", ' ')); 
	cmd_push(cmds, 1, ft_split("grep head", ' ')); 
	cmd_push(cmds, 2, ft_split("./ex arg", ' ')); 
	return NULL;
}

t_cmd	*parser(t_token *tokens){
	//We should read each token until PIPE or end_of_list (; is not included for now)	
	t_cmd		*cmds;
	
	cmds = NULL;	
	//while(tokens){
	tokens = pack_cmd(tokens, &cmds);
	//}
	return cmds;
}
