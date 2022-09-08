/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 18:13:22 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/07 18:14:07 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipes(int **pipes, int pipe_amount)
{
	int	i;

	i = 0;
	while (i < pipe_amount)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	close_pipes(int	**pipes, int pipe_amount)
{
	int	i;

	i = 0;
	while (i < pipe_amount)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
