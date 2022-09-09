/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 16:03:57 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/09 18:19:06 by cgregory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_exit(char **argv)
{
	int	code;
	int	chislo;

	code = 0;
	chislo = 0;
	if (argv[1])
		chislo = ft_atoi(argv[1]);
	if (argv[1] && ft_strncmp(argv[1], "0", 2) != 0 && chislo == 0)
	{
		write(2, "sash: exit: ", 12);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
	else if (argv[1] && argv[2])
	{
		write(2, "sash: exit: too many arguments\n", 31);
		return (code);
	}
	printf("exit\n");
	exit(chislo);
	return (0);
}
