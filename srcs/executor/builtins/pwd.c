/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 16:01:06 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/07 16:01:31 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_pwd(char **argv)
{
	char	buff[MAXPATHLEN];
	int		code;

	(void) argv;
	code = 0;
	if (getcwd(buff, MAXPATHLEN))
		printf("%s\n", buff);
	else
		code = 1;
	return (code);
}
