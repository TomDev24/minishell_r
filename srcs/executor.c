#include "minishell.h"

int	**init_pipes(int pipes_amount){
	int	i = 0;
	int	**res;

	res = (int**)malloc( (sizeof(int*) * pipes_amount) + 1 );
	if (!res){
		perror("CANT ALLOCATE MEM\n");
		exit(1);
	}
	res[pipes_amount] = NULL;
	while (i < pipes_amount){
		res[i] = (int*)malloc(sizeof(int*) * 2);
		pipe(res[i]);
		i++;
	}

	return res;
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

/*
void	free_pids(int *pids, int cmd_amount){
	int	i;

	i = 0;
	while (i < cmd_amount){
		free(pids[i]);
		i++;
	}
} */

void	close_pipes(int	**pipes, int pipe_amount){
	int i;
	
	i = 0;
	while (i < pipe_amount){
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	handle_pipes(t_cmd *cmd, int **pipes, int cmd_amount){
	if (cmd->i == 0){
		//dup2(all->infile, 0);
		dup2(pipes[0][1], 1);
	} else if (cmd->i == cmd_amount - 1){
		dup2(pipes[cmd_amount-2][0], 0); //pipe_amount = cmd_amount -1; and  -1 for indexing
		//dup2(all->outfile, 1);
	} else {
		//read from prev
		dup2(pipes[cmd->i - 1][0], 0);
		//write to next
		dup2(pipes[cmd->i][1], 1);
	}
}

void	run_cmd(t_cmd *cmd, char **cmd_paths, char **envp, int **pipes, int pipe_amount, int cmd_amount){
	char	*path;
	int	allowed;
	
	while(*cmd_paths){
		path = ft_strjoin(*cmd_paths, ft_strjoin("/", cmd->argv[0]));
		allowed = access(path, X_OK);
		if (allowed == 0)
			break;
		cmd_paths++;
	}
	if (allowed < 0)
		return;	
	if (pipe_amount > 0)
		handle_pipes(cmd, pipes, cmd_amount);
	/*
	if (cmd->i == 0){
		//dup2(all->infile, 0);
		dup2(pipes[0][1], 1);
	} else if (cmd->i == cmd_amount - 1){
		dup2(pipes[cmd_amount-2][0], 0); //pipe_amount = cmd_amount -1; and  -1 for indexing
		//dup2(all->outfile, 1);
	} else {
		//read from prev
		dup2(pipes[cmd->i - 1][0], 0);
		//write to next
		dup2(pipes[cmd->i][1], 1);
	} */

	close_pipes(pipes, pipe_amount);
	execve(path, cmd->argv, envp);
}

void	executor(t_cmd *cmds, char **envp, char **cmd_paths){
	int	**pipes;
	int	cmd_amount;
	int	pipe_amount;
	int	*pids;
	int	i;
	
	cmd_amount = cmdlst_size(cmds);
	pipe_amount = cmd_amount - 1;
	pids = (int*)malloc(sizeof(int) * cmd_amount);
	pipes = init_pipes(cmd_amount - 1);
	i = 0;

	//printf("Pipe amount %d\n", cmd_amount - 1);	
	while(cmds){
		pids[i] = fork();
		if (pids[i] == 0)
			run_cmd(cmds, cmd_paths, envp, pipes, pipe_amount, cmd_amount);
		cmds = cmds->next;
	}
	
	close_pipes(pipes, pipe_amount);
	waitpid(-1, NULL, 0);
	free_pipes(pipes, pipe_amount);	
	free(pids);
}
