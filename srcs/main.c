#include "minishell.h"

t_global	mshell;

void    sigint_handler(int num)
{
    (void) num;
    waitpid(-1, NULL, 0);
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int	main(int argc, char **argv, char **envp){
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	init_hash_envp(envp);
	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(argv[2], envp);

    mshell.s_int.sa_handler = sigint_handler;
    sigaction(SIGINT, &mshell.s_int, NULL);
    signal(SIGQUIT, SIG_IGN);

	while(1 && argc == 1){
		tokens = NULL;
		cmds = NULL;
		line = readline(">>");
        if (line == NULL)
        {
            printf("exit\n");
            exit(0);
        }
		add_history(line);
		tokens = lexer(line);
		pretty_lexer(tokens);	
		//print_tokens(tokens);

		cmds = parser(&tokens);	
		print_cmds(cmds);
		executor(cmds, envp);
		//free_tokens(tokens);
		//free_cmds(cmds);
	}
	return 0;
}
