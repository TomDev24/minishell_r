#include "minishell.h"

t_global	mshell;

void	sighandler_prepare()
{
	sigemptyset(&mshell.newset);
	sigaddset(&mshell.newset, SIGINT);
	sigaddset(&mshell.newset, SIGQUIT);
	mshell.s_int.sa_handler = sigint_handler;
	mshell.s_int.sa_mask = SIGINT;
	sigaction(SIGINT, &mshell.s_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

//we should free
//1)line
//2)tokens
//3)cmds
int	main(int argc, char **argv, char **envp){
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	init_hash_envp(envp);
	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(argv[2], envp);
	set_param_tty();
	sighandler_prepare();
	while(1 && argc == 1){
		tokens = NULL;
		cmds = NULL;
		line = readline("sash>");
		if (line == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		add_history(line);
		tokens = lexer(line);
		//pretty_lexer(tokens);	
		//print_tokens(tokens);

		cmds = parser(&tokens);	
		//print_cmds(cmds);
		executor(cmds);
		free_tokens(tokens);
		free_cmds(cmds);
	}
	unset_param_tty();
	ht_delete(&mshell.hash_envp);	
	return 0;
}
