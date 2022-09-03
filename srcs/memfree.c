#include "minishell.h"

//FREE FUNCTIONS SHOULD REWRITTEN AND PLACED ELSEWHERE

//Free list of tokens and 
//1)value
//2)
void	free_tkn(t_token **tkn){
	t_token *tmp;

	if (!*tkn)
		return;
	tmp = *tkn;
	if (*tmp->value != 0)
		free(tmp->value);
	free(*tkn);
	*tkn = NULL;

}
void	free_tokens(t_token *tokens){
	t_token	*tmp;

	while(tokens){
		tmp = tokens;
		tokens = tokens->next;
		free_tkn(&tmp);
	}
}

void	free_arr(char **line){
	while(*line){
		free(*line);
		line++;
	}
	free(line);
}

//typedef struct s_cmd{
//	int		i;
//	char		*infile;
//	char		*outfile;
//
//	char		**argv;  FREE ONLY **
//	t_token		*cmd;
//	t_list		*args; 	 FREE ONLY *, not content
//	struct s_cmd	*next;
//}			t_cmd;
void	free_cmds(t_cmd *cmds){
	t_cmd	*tmp;
	t_list	*tmp2;

	while(cmds){
		tmp = cmds;
		cmds = cmds->next;

		free(tmp->argv);
		while(tmp->args){
			tmp2 = tmp->args;
			tmp->args = tmp->args->next;
			free(tmp2);
		}
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
