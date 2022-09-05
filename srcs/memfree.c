#include "minishell.h"

//FREE FUNCTIONS SHOULD REWRITTEN AND PLACED ELSEWHERE

void	free_context(t_stack **context){
	t_list	*elements;
	t_list	*tmp;
	t_stack *ctx;

	ctx = *context;
	elements = ctx->elements;
	(*context)->q_type = 0;
	while (elements){
		tmp = elements;
		elements = elements->next;
		free(tmp);
	}
	free(*context);
	*context = NULL;
}

void	free_context_elements(t_stack *context, t_token *st_token, t_token *en_token){
	t_list	*head;
	t_token	*tmp;

	head = context->elements;
	if (head->next->content == st_token)
		head = head->next;
	while(st_token != en_token){
		tmp = st_token;
		st_token = st_token->next;
		if (head && head->content == tmp){
			free_tkn((t_token**)&head->content);
			head = head->next;
		}
		else
			free_tkn(&tmp);
	}
	if (head && head->content == st_token)
		free_tkn((t_token**)&head->content);
	else
		free_tkn(&st_token);
	
	/*
	t_token	*st_token;
	t_list	*head;

	head = context->elements;
	st_token = head->content;
	if (st_token->type != context->q_type)
		head = head->next;
	
	free_tkn((t_token**)&head->content);
	head = head->next;
	st_token = head->content;
	while(st_token->type != context->q_type){
		free_tkn((t_token**)&head->content);
		head = head->next;
		st_token = head->content;
	}
	free_tkn((t_token**)&head->content);
	*/
}


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

void	free_arr(char **arr){
	int	i;

	i = 0;
	while(arr[i]){
		free(arr[i]);
		i++;
	}
	free(arr);
}

//typedef struct s_cmd{
//	int		i;
//	char		*infile;
//	char		*outfile;
//
//	t_list		*redirs; FREE * and content
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
		while(tmp->redirs){	
			tmp2 = tmp->redirs;
			tmp->redirs = tmp->redirs->next;
			free(tmp2->content);
			free(tmp2);
		}
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
		close(pipes[i][1]);
		i++;
	}
}
