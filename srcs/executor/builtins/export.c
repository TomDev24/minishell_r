/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 16:03:00 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/08 10:37:09 by tom              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export(void)
{
	char	**envp;
	char	*c;
	int		i;

	i = -1;
	envp = hash_to_array(g_mshell.env, ht_size(g_mshell.env));
	envp = sort_array(envp, array_size(envp));
	while (envp[++i])
	{
		c = ft_strchr(envp[i], '=');
		*(c) = 0;
		if (ft_strncmp(c + 1, "iDQpaUqWalAZdTzUhRWO.#7J1Sbm", 28) == 0)
			printf("declare -x %s\n", envp[i]);
		else
		{
			printf("declare -x %s", envp[i]);
			printf("=");
			printf("\"%s\"\n", c + 1);
		}
	}
	free_arr(envp);
}

int	b_export(char **argv)
{
	int		code;
	char	*c;

	code = 0;
	if (!argv[1])
		print_export();
	while (*(++argv))
	{
		c = ft_strchr(*argv, '=');
		if (!c)
			ht_set(g_mshell.env, *argv, "iDQpaUqWalAZdTzUhRWO.#7J1Sbm");
		else if (*(c + 1) == 0)
		{
			*c = 0;
			ht_set(g_mshell.env, *argv, "");
		}
		else
		{
			*c = 0;
			ht_set(g_mshell.env, *argv, c + 1);
		}
	}
	return (code);
}
