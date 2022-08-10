#include "minishell.h"

t_global	mshell;

char    **parse_envp(char **envp){
	char **res;
	int i = 0;

	res = NULL;
	while(envp[i]){
		if (ft_strncmp(envp[i], "PATH", 4) == 0){
			res = ft_split(ft_strchr(envp[i], '=') + 1, ':');
			break;
		}
		i++;
	}
	return res;
}

int	main(int argc, char **argv, char **envp){
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	init_hash_envp(envp);
	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(argv[2], envp);

	while(1 && argc == 1){
		tokens = NULL;
		cmds = NULL;
		line = readline(">>");
		add_history(line);
		tokens = lexer(line);
		pretty_lexer(tokens);	
		//print_tokens(tokens);

		cmds = parser(tokens);	
		print_cmds(cmds);
		executor(cmds, envp);
		//free_tokens(tokens);
		//free_cmds(cmds);
	}
	return 0;
}
