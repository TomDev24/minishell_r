/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 19:23:41 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 10:37:09 by tom              ###   ########.fr       */
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
//	int	i;
//	i = chdir(argv);
//	printf("i = %d\n", i);
//	if (i != 0)
	if (chdir(argv) != 0)
	{
		write(2, "sash: cd: ", 10);
//		printf("argv: %s\n", argv);
		write(2, argv, ft_strlen(argv));
		write(2, ": ", 2);
		perror("");
//		printf("Error: %s\n", strerror(errno));
	}
}

int	b_cd(char **argv)
{
	char	*vars[3];

	vars[0] = NULL;
	vars[0] = getcwd(vars[0], 1000);
//	printf("getcwd before action: %s\n", curr);
	if (argv[0] && argv[1] && argv[2])
		printf("sash: cd: too many arguments\n");
	else if (!argv[1] || (argv[1] && ft_strncmp(argv[1], "~", 2) == 0))
	{
		vars[2] = ht_get(g_mshell.env, "HOME");
		if (!vars[2])
			err_msg("HOME");
		else
			ht_set(g_mshell.env, "OLDPWD", vars[0]);
		chdir(vars[2]);
	}
	else if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
	{
		vars[1] = ht_get(g_mshell.env, "OLDPWD");
		if (!vars[1])
			err_msg("OLDPWD");
		else
			ht_set(g_mshell.env, "OLDPWD", vars[0]);
		chdir(vars[1]);
	}
	else
	{
		ht_set(g_mshell.env, "OLDPWD", vars[0]);
		chdir_check_error(argv[1]);
	}
	free(vars[0]);
//	printf("errno = %d\n", errno);
//	printf("exit_code = %d\n", errno/errno);
	return (0);
}
