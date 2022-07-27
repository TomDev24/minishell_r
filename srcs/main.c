#include "minishell.h"

int	main(int argc, char **argv, char **envp){
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	tokens = NULL;
	cmds = NULL;
	line = readline(">>");
	tokens = lexer(line);
	//pretty_lexer(tokens);
	//print_tokens(tokens);
	
	cmds = parser(tokens);	
	print_cmds(cmds);

	return 0;
}
