/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 19:18:22 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 17:15:15 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_error_code(void)
{
	if (g_mshell.exit_code < 0)
	{
		g_mshell.exit_code *= -1;
		return (1);
	}
	return (0);
}

void	m_error(int code, char *msg)
{
	ft_putstr_fd("sash: ", 2);
	if (msg && *msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(": ", 2);
	}
	g_mshell.exit_code = code;
	if (code == 127)
		ft_putstr_fd("command not found\n", 2);
	else if (code == -5)
		ft_putstr_fd("incorrect pipeline\n", 2);
	else if (code == -6)
		ft_putstr_fd("incorrect quotes\n", 2);
	else if (code == -7)
		ft_putstr_fd("command not provided\n", 2);
	else if (code == -8)
		ft_putstr_fd("syntax error\n", 2);
	else
		ft_putstr_fd("error\n", 2);
	if (code < 0)
		return ;
	exit(code);
}
