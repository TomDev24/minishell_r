/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 18:46:51 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 11:33:25 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//cmd->outfile etc. is outdated needs refactor
int	handle_pipes(t_cmd *cmd, int **pipes, int cmd_amount)
{
	if (cmd_amount - 1 <= 0)
		return (1);
	if (cmd->i == 0)
	{
		if (!cmd->outfile)
			dup2(pipes[0][1], 1);
	}
	else if (cmd->i == cmd_amount - 1)
	{
		if (!cmd->infile)
			dup2(pipes[cmd_amount - 2][0], 0);
	}
	else
	{
		if (!cmd->infile)
			dup2(pipes[cmd->i - 1][0], 0);
		if (!cmd->outfile)
			dup2(pipes[cmd->i][1], 1);
	}
	close_pipes(pipes, cmd_amount - 1);
	return (1);
}

int	exec_builtin(t_cmd *cmd, int **pipes, int pipe_amount, int cmd_amount)
{
	int	code;
	int	save_fd;
	int	(*func)(char**);

	code = -42;
	save_fd = -1;
	func = find_builtin(cmd->argv[0]);
	if (func == NULL)
		return (code);
	save_fd = dup(1);
	handle_redirects(cmd);
	handle_pipes(cmd, pipes, cmd_amount);
	code = func(cmd->argv);
	g_mshell.exit_code = code;
	if (pipe_amount > 0)
		exit(code);
	else
		dup2(save_fd, 1);
	return (code);
}

void	run_cmd(t_cmd *cmd, int **pipes, int cmd_amount, int i)
{
	char	*path;
	int		allowed;
	char	**cmd_paths;
	char	*tmp;

	cmd_paths = parse_envp();
	allowed = access(cmd->argv[0], X_OK);
	if (allowed == 0)
		path = cmd->argv[0];
	while (allowed != 0 && cmd_paths && cmd_paths[++i])
	{
		tmp = ft_strjoin("/", cmd->argv[0]);
		path = ft_strjoin(cmd_paths[i], tmp);
		free(tmp);
		allowed = access(path, X_OK);
		if (allowed != 0)
			free(path);
	}
	free_arr(cmd_paths);
	if (allowed < 0)
		exit(0);
	i = handle_redirects(cmd) * handle_pipes(cmd, pipes, cmd_amount);
	g_mshell.s_quit.sa_handler = sigquit_handler;
	sigaction(SIGQUIT, &g_mshell.s_quit, NULL);
	execve(path, cmd->argv, hash_to_array(g_mshell.env, ht_size(g_mshell.env)));
}

void	executor(t_cmd *cmds, int cmd_amount)
{
	t_exec	exec;
	int		code;

	code = 0;
	if (cmd_amount == 0)
		return ;
	pre_process(&exec, cmd_amount);
	while (cmds)
	{
		if (cmd_amount == 1)
		{
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code != -42)
				break ;
		}
		exec.pids[0] = fork();
		if (exec.pids[0] == 0)
		{
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code == -42)
				run_cmd(cmds, exec.pipes, cmd_amount, -1);
		}
		cmds = cmds->next;
	}
	post_process(&exec, cmd_amount);
}