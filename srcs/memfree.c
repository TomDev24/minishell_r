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

void	free_pipes(int **pipes, int pipe_amount){
	int	i;

	i = 0;
	while (i < pipe_amount){
		free(pipes[i]);
		i++;
	}
	
	free(pipes);
}

void	close_pipes(int	**pipes, int pipe_amount){
	int i;
	
	i = 0;
	while (i < pipe_amount){
		close(pipes[i][0]);
		close(pipes[i][1]); i++;
	}
}
