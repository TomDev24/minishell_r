#include "minishell.h"

int	**init_pipes(int pipes_amount){
	int	i = 0;
	int	**res; 

	res = (int**)malloc( sizeof(int*) * (pipes_amount + 1) );
	if (!res){
		perror("CANT ALLOCATE MEM\n");
		exit(1);
	}
	//res[pipes_amount] = NULL;
	while (i < pipes_amount){
		res[i] = (int*)malloc(sizeof(int) * 2);	
		if (pipe(res[i]) < 0)
			perror("PIPE ERROR");
		i++;
	}

	return res;
}

void	handle_pipes(t_cmd *cmd, int **pipes, int cmd_amount){
	if (cmd->i == 0){
		if (!cmd->outfile)
			dup2(pipes[0][1], 1);
	} else if (cmd->i == cmd_amount - 1){
		if (!cmd->infile)
			dup2(pipes[cmd_amount-2][0], 0); //pipe_amount = cmd_amount -1; and  -1 for indexing
	} else {
		//read from prev
		if (!cmd->infile)
			dup2(pipes[cmd->i - 1][0], 0);
		//write to next
		if (!cmd->outfile)
			dup2(pipes[cmd->i][1], 1);
	}
}

void	handle_redirects(t_cmd *cmd){
	int	fd;

	//is duplecation of descriptor, second time is good thing?
	if (cmd->infile){
		fd = open(cmd->infile, O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	if (cmd->outfile){
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0664);
		dup2(fd, 1);
		close(fd);
	}
}


void	pre_exec(t_cmd *cmd, int **pipes, int pipe_amount, int cmd_amount){
	handle_redirects(cmd);
	if (pipe_amount > 0)
		handle_pipes(cmd, pipes, cmd_amount);

	close_pipes(pipes, pipe_amount);

}

int	exec_builtin(t_cmd *cmd, int **pipes, int pipe_amount, int cmd_amount){
	int	code;
	int	save_fd;
	
	code = -42;
	save_fd = -1;
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0){
		save_fd = dup(1);
		pre_exec(cmd, pipes, pipe_amount, cmd_amount);
		code = b_echo(cmd->argv);
		if (pipe_amount > 0)
			exit(1);
		else
			dup2(save_fd, 1);

	} else if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0){
		pre_exec(cmd, pipes, pipe_amount, cmd_amount);
		code = b_pwd();
		if (pipe_amount > 0)
			exit(1);
	} else if (ft_strncmp(cmd->argv[0], "env", 3) == 0){
		pre_exec(cmd, pipes, pipe_amount, cmd_amount);
		code = b_env();
		if (pipe_amount > 0)
			exit(1);
	} else if (ft_strncmp(cmd->argv[0], "export", 6) == 0){
		save_fd = dup(1);
		pre_exec(cmd, pipes, pipe_amount, cmd_amount);
		code = b_export(cmd->argv);
		if (pipe_amount > 0)
			exit(1);
		else
			dup2(save_fd, 1); } else if (ft_strncmp(cmd->argv[0], "unset", 5) == 0){
		pre_exec(cmd, pipes, pipe_amount, cmd_amount);
		code = b_unset(cmd->argv);
		if (pipe_amount > 0)
			exit(1);
	}

	return code;
}


void	run_cmd(t_cmd *cmd, char **envp, int **pipes, int pipe_amount, int cmd_amount){
	char	*path;
	int	allowed;
	char	**cmd_paths;

	cmd_paths = parse_envp(envp);
	while(*cmd_paths){
		path = ft_strjoin(*cmd_paths, ft_strjoin("/", cmd->argv[0]));
		allowed = access(path, X_OK);
		//printf("PAth %s access %d\n", path, allowed);
		if (allowed == 0)
			break;
		cmd_paths++;
	}
	if (allowed < 0)
		return;	

	pre_exec(cmd, pipes, pipe_amount, cmd_amount);
	execve(path, cmd->argv, envp);
}

int	post_process(t_exec *exec, int cmd_amount){
	int	j;

	j = -1;
	close_pipes(exec->pipes, exec->pipe_amount);
	while (++j < cmd_amount)
		waitpid(exec->pids[j], NULL, 0);
	//free_pipes(exec->pipes, exec->pipe_amount);	
	free(exec->pids);
	
	return 1;
}

void	pre_process(t_exec *exec, int cmd_amount){
	int	**pipes;
	int	pipe_amount;
	int	*pids;

	pipe_amount = cmd_amount - 1;
	pids = (int*)malloc(sizeof(int) * cmd_amount);
	pipes = init_pipes(pipe_amount);
	
	exec->pipe_amount = pipe_amount;
	exec->pids = pids;
	exec->pipes = pipes;	
}

void	executor(t_cmd *cmds, char **envp){
	t_exec	exec;	
	int	cmd_amount;
	int	code;
	int	i;
	
	cmd_amount = cmdlst_size(cmds);	
	i = 0;
	code = 0;
	if (cmd_amount == 0)
		return; //should return code?
	pre_process(&exec, cmd_amount);
	//here for now exec_buitlin calls two times
	while(cmds){
		if (cmd_amount == 1){
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code != -42)
				return;
		}
		exec.pids[i] = fork();
		if (exec.pids[i] == 0){
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code == -42)
				run_cmd(cmds,  envp, exec.pipes, exec.pipe_amount, cmd_amount);
		}
		cmds = cmds->next;
	}
	post_process(&exec, cmd_amount);
}
