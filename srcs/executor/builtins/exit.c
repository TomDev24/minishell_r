/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 16:03:57 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/07 16:04:16 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_exit(char **argv)
{
	int	code;
	int	chislo;

	code = 0;
	chislo = 0;
//	printf("builtin cmd: exit\n");
	if (argv[1])
		chislo = ft_atoi(argv[1]);
	//	if chislo is not number
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
//	printf("chislo = %d\n", chislo);
	printf("exit\n");
	exit(chislo);
	return (code);
}
