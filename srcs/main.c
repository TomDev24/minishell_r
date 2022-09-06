#include "minishell.h"

t_global	mshell;

void	sighandler_prepare()
{
	sigemptyset(&mshell.newset);
	sigaddset(&mshell.newset, SIGINT);
	sigaddset(&mshell.newset, SIGQUIT);
	mshell.s_int.sa_handler = sigint_handler;
	//mshell.s_int.sa_mask = SIGINT;
	sigaction(SIGINT, &mshell.s_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	post_process(t_exec *exec, int cmd_amount){
	int	j;
	int	status;

	j = -1;
	close_pipes(exec->pipes, exec->pipe_amount);
	while (++j < cmd_amount && exec->pids[j] >= 0)
	{
		waitpid(exec->pids[j], &status, 0);
		//printf("status = %d\n", WEXITSTATUS(status));
		mshell.exit_code = WEXITSTATUS(status);
	}
	free_pipes(exec->pipes, exec->pipe_amount);	
	free(exec->pids);
	return 1;
}

void	pre_process(t_exec *exec, int cmd_amount){
	int	**pipes;
	int	pipe_amount;
	int	*pids;
	int	i;

	pipe_amount = cmd_amount - 1;
	i = -1;
	pids = (int*)malloc(sizeof(int) * cmd_amount);
	if (!pids)
		m_error(1);
	while (++i < cmd_amount)
		pids[i] = -1;	
	pipes = init_pipes(pipe_amount);	
	exec->pipe_amount = pipe_amount;
	exec->pids = pids;
	exec->pipes = pipes;	
}

void	sub_main(char *line){
	t_token		*tokens;
	t_token		*cur_tokens;
	t_token		*tmp;
	t_cmd		*cmds;
	t_exec		exec;	
	int		cmd_amount;

	tokens = NULL;
	cmds = NULL;
	tokens = lexer(line);
	cmd_amount = get_last_token_i(tokens) + 1;
	cur_tokens = tokens;
	pre_process(&exec, cmd_amount);
	while(cur_tokens){
		if (cur_tokens->next && cur_tokens->next->type == PIPE){
			tmp = cur_tokens->next;
			cur_tokens->next = NULL;
			cmds = parser(&tokens);	
			//print_cmds(cmds);
			executor(cmds, &exec, cmd_amount, cur_tokens->i);
			free_tokens(tokens);
			free_cmds(cmds);
			tokens = tmp->next;
			cur_tokens = tmp->next;
			free_tkn(&tmp);
			continue;
		} else if(!cur_tokens->next){
			cmds = parser(&tokens);	
			executor(cmds, &exec, cmd_amount, cur_tokens->i);
			free_tokens(tokens);
			free_cmds(cmds);
		}
		cur_tokens= cur_tokens->next;
	}		
	post_process(&exec, cmd_amount);
}

int	main(int argc, char **argv, char **envp){
	char		*line;

	init_hash_envp(envp);

	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(argv[2]);

	set_param_tty();
	sighandler_prepare();

	while(1 && argc == 1){
		line = readline("sash>");
		if (line == NULL){
			printf("exit\n");
			exit(0);
		}
		add_history(line);
		sub_main(line);
	}

	unset_param_tty();
	ht_delete(&mshell.hash_envp);	
	return 0;
}
