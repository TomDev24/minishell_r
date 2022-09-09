/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 19:23:41 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/09 18:18:59 by cgregory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	err_msg(char *argv)
{
	write(2, "sash: cd: ", 10);
	write(2, argv, ft_strlen(argv));
	write(2, " not set\n", 9);
}

static void	chdir_check_error(char *argv)
{
	if (chdir(argv) != 0)
	{
		write(2, "sash: cd: ", 10);
		write(2, argv, ft_strlen(argv));
		write(2, ": ", 2);
		perror("");
	}
}

static void	action_1(char **vars)
{
	vars[2] = ht_get(g_mshell.env, "HOME");
	if (!vars[2])
		err_msg("HOME");
	else
		ht_set(g_mshell.env, "OLDPWD", vars[0]);
	chdir(vars[2]);
}

static void	action_2(char **vars)
{
	vars[1] = ht_get(g_mshell.env, "OLDPWD");
	if (!vars[1])
		err_msg("OLDPWD");
	else
		ht_set(g_mshell.env, "OLDPWD", vars[0]);
	chdir(vars[1]);
}

int	b_cd(char **argv)
{
	char	*vars[3];

	vars[0] = NULL;
	vars[0] = getcwd(vars[0], 1000);
	if (argv[0] && argv[1] && argv[2])
		printf("sash: cd: too many arguments\n");
	else if (!argv[1] || (argv[1] && ft_strncmp(argv[1], "~", 2) == 0))
	{
		action_1(vars);
	}
	else if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
	{
		action_2(vars);
	}
	else
	{
		ht_set(g_mshell.env, "OLDPWD", vars[0]);
		chdir_check_error(argv[1]);
	}
	free(vars[0]);
	return (0);
}
