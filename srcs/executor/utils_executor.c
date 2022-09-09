/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 17:56:18 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/09 13:59:42 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_envp(void)
{
	char	**res;
	char	*paths;

	res = NULL;
	paths = ht_get(g_mshell.env, "PATH");
	if (paths)
		res = ft_split(paths, ':');
	return (res);
}

int	**init_pipes(int pipes_amount)
{
	int	i;
	int	**res;

	i = 0;
	res = (int **)malloc(sizeof(int *) * (pipes_amount + 1));
	if (!res)
		m_error(1, "");
	while (i < pipes_amount)
	{
		res[i] = (int *)malloc(sizeof(int) * 2);
		if (!res[i])
			m_error(1, "");
		if (pipe(res[i]) < 0)
			m_error(3, "");
		i++;
	}
	return (res);
}

int	(*find_builtin(char *name))(char **value)
{
	if (ft_strncmp("echo", name, ft_strlen(name) + 1) == 0)
		return (&b_echo);
	if (ft_strncmp("pwd", name, ft_strlen(name) + 1) == 0)
		return (&b_pwd);
	if (ft_strncmp("env", name, ft_strlen(name) + 1) == 0)
		return (&b_env);
	if (ft_strncmp("export", name, ft_strlen(name) + 1) == 0)
		return (&b_export);
	if (ft_strncmp("unset", name, ft_strlen(name) + 1) == 0)
		return (&b_unset);
	if (ft_strncmp("exit", name, ft_strlen(name) + 1) == 0)
		return (&b_exit);
	if (ft_strncmp("cd", name, ft_strlen(name) + 1) == 0)
		return (&b_cd);
	return (NULL);
}

int	post_process(t_exec *exec, int cmd_amount)
{
	int	j;
	int	status;

	j = -1;
	close_pipes(exec->pipes, exec->pipe_amount);
	while (++j < cmd_amount && exec->pids[j] >= 0)
	{
		waitpid(-1, &status, 0);
		g_mshell.exit_code = WEXITSTATUS(status);
	}
	free_pipes(exec->pipes, exec->pipe_amount);
	free(exec->pids);
	return (1);
}

void	pre_process(t_exec *exec, int cmd_amount)
{
	int	**pipes;
	int	pipe_amount;
	int	*pids;
	int	i;

	pipe_amount = cmd_amount - 1;
	i = -1;
	pids = (int *)malloc(sizeof(int) * cmd_amount);
	if (!pids)
		m_error(1, "");
	while (++i < cmd_amount)
		pids[i] = -1;
	pipes = init_pipes(pipe_amount);
	exec->pipe_amount = pipe_amount;
	exec->pids = pids;
	exec->pipes = pipes;
}
