#include "minishell.h"

//we should parse hash_envp
//and we should free it, after we done
char    **parse_envp(){
	char	**res;
	char	*paths;

	res = NULL;
	paths = ht_get(mshell.hash_envp, "PATH");
	if (paths)
		res = ft_split(paths, ':');
	return res;
}

int	**init_pipes(int pipes_amount){
	int	i = 0;
	int	**res; 

	res = (int**)malloc( sizeof(int*) * (pipes_amount + 1) );
	if (!res)
		m_error(1);
	//res[pipes_amount] = NULL;
	while (i < pipes_amount){
		res[i] = (int*)malloc(sizeof(int) * 2);	
		if (!res[i])
			m_error(1);
		if (pipe(res[i]) < 0)
			m_error(3);
		i++;
	}

	return res;
}

//cmd->outfile etc. is outdated needs refactor
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
	t_redir	*r;
	t_list	*redirects;
	int	std_in;
			
	std_in = dup(0);
	redirects = cmd->redirs;	
	while(redirects){
		r = redirects->content;
		if (r->type == IN){
			fd = open(r->filen, O_RDONLY);
			dup2(fd, 0);
			close(fd);
		}
		else if (r->type == OUT){
			fd = open(r->filen, O_TRUNC | O_CREAT | O_WRONLY, 0664);
			dup2(fd, 1);
			close(fd);
		}
		else if (r->type == ININ){
			dup2(std_in, 0);
			here_doc(r->filen);
		}
		redirects = redirects->next;
	}
}

int (*find_builtin(char *name))(char **){
	if (ft_strncmp("echo", name, ft_strlen(name) + 1) == 0)
		return &b_echo;
	if (ft_strncmp("pwd", name, ft_strlen(name) + 1) == 0)
		return &b_pwd;
	if (ft_strncmp("env", name, ft_strlen(name) + 1) == 0)
		return &b_env;
	if (ft_strncmp("export", name, ft_strlen(name) + 1) == 0)
		return &b_export;
	if (ft_strncmp("unset", name, ft_strlen(name) + 1) == 0)
		return &b_unset;
	if (ft_strncmp("exit", name, ft_strlen(name) + 1) == 0)
		return &b_exit;
	if (ft_strncmp("cd", name, ft_strlen(name) + 1) == 0)
		return &b_cd;
	return NULL;
}

int	exec_builtin(t_cmd *cmd, int **pipes, int pipe_amount, int cmd_amount){
	int	code;
	int	save_fd;
	int	(*func)(char**);
	
	code = -42;
	save_fd = -1;

	func = find_builtin(cmd->argv[0]);
	if(func == NULL)
		return code;

	save_fd = dup(1);
	//pre_exec(cmd, pipes, pipe_amount, cmd_amount);
	handle_redirects(cmd);
	if (pipe_amount > 0)
		handle_pipes(cmd, pipes, cmd_amount);
	close_pipes(pipes, pipe_amount);

	code = func(cmd->argv);
//	printf("cmd->argv: %s\n", cmd->argv[0]);
	update_mshell(code, cmd->i);
	if (pipe_amount > 0)
		exit(code);
	else
		//printf does not write immedialty to fd!!!(it has buffer)
		dup2(save_fd, 1);

	return code;
}


void	run_cmd(t_cmd *cmd, int **pipes, int pipe_amount, int cmd_amount){
	char	*path;
	int	allowed;
	char	**cmd_paths;
	char	*tmp;
	int	i;
	char	**envp;

	i = 0;
	cmd_paths = parse_envp();
	while(cmd_paths && cmd_paths[i]){
		tmp = ft_strjoin("/", cmd->argv[0]);
		path = ft_strjoin(cmd_paths[i], tmp);
		free(tmp);
		allowed = access(path, X_OK);
		//printf("PAth %s access %d\n", path, allowed);
		if (allowed == 0)
			break;
		i++;
		if (cmd_paths[i]){
			free(path);
		}
	}
	if (cmd_paths)
		free_arr(cmd_paths);
	//checking if cmd itself is path // is check on '.' or '/' required?
	if (allowed < 0){
		allowed = access(cmd->argv[0], X_OK);
		if (allowed == 0){
			free(path); // freeing old path
			path = cmd->argv[0];
		}
		else
			exit(0);
	}

	//pre_exec(cmd, pipes, pipe_amount, cmd_amount);
	handle_redirects(cmd);
	if (pipe_amount > 0)
		handle_pipes(cmd, pipes, cmd_amount);

	close_pipes(pipes, pipe_amount);
	//check if envp is null required
	envp = hash_to_array(mshell.hash_envp);
	//path is heap alocated is it get lost in fork?
	execve(path, cmd->argv, envp);
}

int	post_process(t_exec *exec, int cmd_amount){
	int	j;

	j = -1;
	close_pipes(exec->pipes, exec->pipe_amount);
	while (++j < cmd_amount && exec->pids[j]){
		waitpid(exec->pids[j], NULL, 0);
		update_mshell(j+20, j);
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

void	executor(t_cmd *cmds){
	t_exec	exec;	
	int	cmd_amount;
	int	code;
	int	i;
	
	i = 0;
	code = 0;
	cmd_amount = cmdlst_size(cmds);	
	if (cmd_amount == 0)
		return; //should return code?
//printf("inside executor\n");
	pre_process(&exec, cmd_amount);
	//here for now exec_buitlin calls two times
	while(cmds){
		if (cmd_amount == 1){
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code != -42){
				post_process(&exec, cmd_amount);
				return;
			}
		}
		exec.pids[i] = fork();
		if (exec.pids[i] == 0){
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code == -42)
				run_cmd(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
		}
		cmds = cmds->next;
	}
	post_process(&exec, cmd_amount);
}
