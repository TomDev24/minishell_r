/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 18:46:51 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/09 14:39:25 by dbrittan         ###   ########.fr       */
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

char	*find_path(t_cmd *cmd, int *allowed, char **cmd_paths)
{
	char	*path;
	char	*tmp;
	int		i;

	path = "";
	i = -1;
	while (*allowed != 0 && cmd_paths && cmd_paths[++i])
	{
		tmp = ft_strjoin("/", cmd->argv[0]);
		path = ft_strjoin(cmd_paths[i], tmp);
		free(tmp);
		*allowed = access(path, X_OK);
		if (*allowed != 0)
			free(path);
	}
	free_arr(cmd_paths);
	return (path);
}

void	run_cmd(t_cmd *cmd, int **pipes, int cmd_amount)
{
	char	*path;
	int		allowed;
	char	**cmd_paths;
	char	*shlvl;
	char	*m;

	cmd_paths = parse_envp();
	allowed = access(cmd->argv[0], X_OK);
	if (allowed == 0)
		path = cmd->argv[0];
	m = ft_strchr(path, 'm');
	if (m && ft_strncmp(m, "minishell", 10) == 0)
	{
		shlvl = ft_itoa(ft_atoi(ht_get(g_mshell.env, "SHLVL")) + 1);
		ht_set(g_mshell.env, "SHLVL", shlvl);
	}
	path = find_path(cmd, &allowed, cmd_paths);
	if (allowed < 0)
		m_error(127, cmd->argv[0]);
	cmd_amount = handle_redirects(cmd) * handle_pipes(cmd, pipes, cmd_amount);
	g_mshell.s_quit.sa_handler = sigquit_handler;
	sigaction(SIGQUIT, &g_mshell.s_quit, NULL);
	if (g_mshell.exit_code == -9)
		exit(9);
	execve(path, cmd->argv, hash_to_array(g_mshell.env, ht_size(g_mshell.env)));
}

void	executor(t_cmd *cmds, int cmd_amount)
{
	t_exec	exec;
	int		code;
	int		i;

	code = 0;
	i = -1;
	if (handle_error_code() || cmd_amount == 0)
		return ;
	pre_process(&exec, cmd_amount);
	if (cmd_amount == 1)
		code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
	while (cmds && (cmd_amount > 1 || code == -42))
	{
		exec.pids[++i] = fork();
		if (exec.pids[i] == 0)
		{
			code = exec_builtin(cmds, exec.pipes, exec.pipe_amount, cmd_amount);
			if (code == -42)
				run_cmd(cmds, exec.pipes, cmd_amount);
		}
		cmds = cmds->next;
	}
	post_process(&exec, cmd_amount);
}
