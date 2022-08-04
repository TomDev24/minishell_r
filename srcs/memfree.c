#include "minishell.h"

//FREE FUNCTIONS SHOULD REWRITTEN AND PLACED ELSEWHERE
void	free_tokens(t_token *tokens){
	t_token	*tmp;

	while(tokens){
		tmp = tokens;
		tokens = tokens->next;
		free(tmp);
	}
}

void	free_arr(char **line){
	while(*line){
		free(*line);
		line++;
	}
	free(line);
}

void	free_cmds(t_cmd *cmds){
	//not best way
	t_cmd	*tmp;
	while(cmds){
		tmp = cmds;
		cmds = cmds->next;
		free(tmp->infile);
		free(tmp->outfile);
		//free_arr(tmp->argv);
		//ft_lstclear(
		free(tmp);
	}
}
