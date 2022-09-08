/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 16:01:54 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/08 10:37:09 by tom              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_env(char	**argv)
{
	int		code;
	int		i;
	char	**s;
	char	*c;

	(void) argv;
	code = 0;
	i = -1;
	s = hash_to_array(g_mshell.env, ht_size(g_mshell.env));
	while (s[++i])
	{
		c = ft_strchr(s[i], '=');
		if (ft_strncmp(c + 1, "iDQpaUqWalAZdTzUhRWO.#7J1Sbm", 28) != 0)
			printf("%s\n", s[i]);
	}
	free_arr(s);
	return (code);
}
