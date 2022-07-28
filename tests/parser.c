#include <stdlib.h>
#include "minishell.h"

char	*test_read_file(FILE *fp){
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	if ((read = getline(&line, &len, fp)) != -1 && read > 0) 
		return line;

	return line;
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

void	p_cmds(t_cmd *cmds){
	char line[500];
	
	while(cmds){
		sprintf(line, "i:%d |CMD:%s |INFILE:%s |OUTFILE:%s |ARGV:%s\n", cmds->i, cmds->cmd->value, 
				cmds->infile, cmds->outfile, array_to_line(cmds->argv)); //arr_to_line add space at the end
		printf(line);
		cmds = cmds->next;
	}
}

int	check_cmds_old(t_cmd *cmds){
	//its easier using string compare	

	p_cmds(cmds);
	return 0;
}

int	main(int argc, char **argv){
	FILE * fp;
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	tokens = NULL;
	cmds = NULL;
	fp = fopen("./tests/cases_parser", "r");
	while ( (line=test_read_file(fp)) != NULL && *line){
		tokens = lexer(line);
		cmds = parser(tokens);
		//TESTER
		check_cmds_old(cmds);
	}
	
	return 0;
}
