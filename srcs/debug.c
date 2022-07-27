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

/*
enum	lexer_types{
	CMD,
	ARG,
	PIPE,
	FILEN,
	OUT,
	IN,
	ININ,
	OUTOUT
};
*/

void	print_tokens(t_token *tokens){
	while(tokens){
		printf("//TOKEN//\n");
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

void	print_cmds(t_cmd *cmds){
	while(cmds){
		printf("--CMD--\n");
		printf("INDEX: %d|ARGV: %s\n", cmds->i, array_to_line(cmds->argv));
		cmds = cmds->next;
	}
}

void 	pretty_lexer(t_token *tokens){
	while(tokens){
		printf("%s ", type_to_string(tokens->type));
		tokens = tokens->next;
	}
}
