/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 15:59:20 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/09 10:27:35 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_unset(char **argv)
{
	int	code;

	code = 0;
	if (!argv[1])
		return (code);
	while (*(++argv))
	{
		if (ft_strncmp(*argv, "SHLVL", 6) == 0)
			ht_set(g_mshell.env, "SHLVL", "0");
		else
			ht_del(g_mshell.env, *argv);
	}
	return (code);
}
