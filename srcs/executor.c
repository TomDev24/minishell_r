#include "minishell.h"

int	**init_pipes(int pipes_amount){ int	i = 0; int	**res; 
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

void	free_pipes(int **pipes, int pipe_amount){
	int	i;

	i = 0;
	while (i < pipe_amount){
		free(pipes[i]);
		i++;
	}
	
	free(pipes);
}
void	close_pipes(int	**pipes, int pipe_amount){
	int i;
	
	i = 0;
	while (i < pipe_amount){
		close(pipes[i][0]);
		close(pipes[i][1]); i++;
	}
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
			dup2(save_fd, 1);
	} else if (ft_strncmp(cmd->argv[0], "unset", 5) == 0){
		pre_exec(cmd, pipes, pipe_amount, cmd_amount);
		code = b_unset(cmd->argv);
		if (pipe_amount > 0)
			exit(1);
	}

	return code;
}


void	run_cmd(t_cmd *cmd, char **cmd_paths, char **envp, int **pipes, int pipe_amount, int cmd_amount){
	char	*path;
	int	allowed;
	
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

void	executor(t_cmd *cmds, char **envp){
	int	**pipes;
	int	cmd_amount;
	int	pipe_amount;
	int	*pids;
	int	i;
	char	**cmd_paths;
	
	//cmd_paths = hash_to_array(mshell.hash_envp);
	//printf("%s", *cmd_paths);
	cmd_paths = parse_envp(envp);
	cmd_amount = cmdlst_size(cmds);
	pipe_amount = cmd_amount - 1;
	pids = (int*)malloc(sizeof(int) * cmd_amount);
	pipes = init_pipes(cmd_amount - 1);
	i = 0;

	//printf("Pipe amount %d\n", cmd_amount - 1);	
	if (cmd_amount == 1){
		if (exec_builtin(cmds, pipes, pipe_amount, cmd_amount) == -42){
			pids[i] = fork();
			if (pids[i] == 0){
				run_cmd(cmds, cmd_paths, envp, pipes, pipe_amount, cmd_amount);
			}
		}
		cmds = cmds->next;
	}
	while(cmds){
		pids[i] = fork();
		if (pids[i] == 0)
			if (exec_builtin(cmds, pipes, pipe_amount, cmd_amount) == -42)
				run_cmd(cmds, cmd_paths, envp, pipes, pipe_amount, cmd_amount);
		cmds = cmds->next;
	}
	
	close_pipes(pipes, pipe_amount);
	//waitpid(-1, NULL, 0);
	int j = -1;
	while (++j < cmd_amount)
		waitpid(pids[j], NULL, 0);
	//free_pipes(pipes, pipe_amount);	
	free(pids);
}

