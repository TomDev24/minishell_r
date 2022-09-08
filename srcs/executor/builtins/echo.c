/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 15:59:57 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/07 16:00:24 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_echo(char	**argv)
{
	int	code;
	int	n_flag;

	code = 0;
	n_flag = 0;
	argv++;
	if (*argv && ft_strncmp(*argv, "-n", 3) == 0)
	{
		n_flag = 1;
		argv++;
		while (*argv && ft_strncmp(*argv, "-n", 3) == 0)
			argv++;
	}
	while (*argv)
	{
		ft_putstr_fd(*argv++, 1);
		if (*argv)
			ft_putstr_fd(" ", 1);
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (code);
}
