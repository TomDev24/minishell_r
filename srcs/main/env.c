/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 19:18:33 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/09 20:49:44 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env(char **envp)
{
	t_ht	*ht;
	char	*key;
	char	*tmp;

	g_mshell.env = ht_create();
	g_mshell.exit_code = 0;
	ht = g_mshell.env;
	while (*envp)
	{
		key = *envp;
		tmp = ft_strchr(key, '=');
		*tmp = '\0';
		ht_set(ht, key, tmp + 1);
		*tmp = '=';
		envp++;
	}
}
