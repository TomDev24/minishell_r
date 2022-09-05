#include "minishell.h"

char *type_to_string(int type){
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

static	char *array_to_line(char **arr){
	char *res;
	char *tmp1;
	char *tmp2;

	res = "";
	while(*arr){
		tmp1 = res;
		tmp2 = ft_strjoin(*arr, " ");
		res = ft_strjoin(res, tmp2);
		if (*tmp1)
			free(tmp1);
		if (*tmp2)
			free(tmp2);
		arr++;
	}
	return res;
}

/*
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
*/

void	print_list(t_list *lst){
	t_token	*tkn;

	while(lst){
		tkn = lst->content;
		printf("Token value %s\n", tkn->value);
		lst = lst->next;
	}
}

/* CMDS PRINTER */

void	print_cmds(t_cmd *cmds){
	char	*argv;

	argv = array_to_line(cmds->argv);
	while(cmds){
		printf("--CMD--\n");
		printf("i:%d |CMD:%s |INFILE:%s |OUTFILE:%s |DELIM:%s |ARGV:%s\n", cmds->i, cmds->cmd->value, 
				cmds->infile, cmds->outfile, cmds->eof, argv); //arr_to_line add space at the end
		cmds = cmds->next;
	}
	free(argv);
}

/* TOKENS PRINTER */

void	print_tokens(t_token *tokens){
	while(tokens){
		printf("//TOKEN//\n");
		printf("i: %d ", tokens->i);
		printf("Type: %d  ", tokens->type);
		printf("Value: %s  ", tokens->value);
		printf("Addr: %s\n", tokens->addr);
		tokens = tokens->next;
	}	
}

void 	pretty_lexer(t_token *tokens){
	while(tokens){
		printf("%s ", type_to_string(tokens->type));
		tokens = tokens->next;
	}
}
