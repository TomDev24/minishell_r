#include "minishell.h"

static char *type_to_string(int type){
	if (type == 0)
		return "CMD";
	if (type == 1)
		return "ARG";
	if (type == 2)
		return "PIPE";
	if (type == 3)
		return "FILEN";
	if (type == 4)
		return "OUT";
	if (type == 5)
		return "IN";
	if (type == 6)
		return "ININ";
	if (type == 7)
		return "OUTOUT";
	if (type == 8)
		return "DELIMITER";
	if (type == 9)
		return "Q1";
	if (type == 10)
		return "Q2";

	return NULL;
}

void	print_tokens(t_token *tokens){ while(tokens){ printf("//TOKEN//\n");
		printf("Type: %d  ", tokens->type);
		printf("Value: %s  ", tokens->value);
		printf("Addr: %s\n", tokens->addr);
		tokens = tokens->next;
	}	
}

static	char *array_to_line(char **arr){
	char *res;

	res = "";
	while(*arr){
		res = ft_strjoin(res, ft_strjoin(*arr, " "));
		arr++;
	}
	return res;
}

static	char *list_to_line(t_list *lst){
	char	*res;
	t_token	*el;

	res = "";
	while(lst){
		el = lst->content;
		res = ft_strjoin(res, ft_strjoin(el->value, " "));
		lst = lst->next;
	}
	return res;
}

void	print_cmds(t_cmd *cmds){
	while(cmds){
		printf("--CMD--\n");
		//printf("INDEX: %d|ARGV: %s\n", cmds->i, array_to_line(cmds->argv));
		/*
		while(*(cmds->argv)){
			printf("%s\n", *(cmds->argv));
			(cmds->argv)++;
		}*/
		printf("i:%d |CMD:%s |INFILE:%s |OUTFILE:%s |ARGV:%s\n", cmds->i, cmds->cmd->value, 
				cmds->infile, cmds->outfile, array_to_line(cmds->argv)); //arr_to_line add space at the end
		cmds = cmds->next;
	}
}

void 	pretty_lexer(t_token *tokens){
	while(tokens){
		printf("%s ", type_to_string(tokens->type));
		tokens = tokens->next;
	}
}
