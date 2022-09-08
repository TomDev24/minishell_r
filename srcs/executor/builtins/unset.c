/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 15:59:20 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/08 10:37:09 by tom              ###   ########.fr       */
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
		ht_del(g_mshell.env, *argv);
	return (code);
}
