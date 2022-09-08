/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memfree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:21:13 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 10:29:20 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tkn(t_token **tkn)
{
	t_token	*tmp;

	if (!*tkn)
		return ;
	tmp = *tkn;
	if (tmp->value && *tmp->value != 0)
		free(tmp->value);
	free(*tkn);
	*tkn = NULL;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free_tkn(&tmp);
	}
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_list	*tmp2;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		free(tmp->argv);
		while (tmp->redirs)
		{
			tmp2 = tmp->redirs;
			tmp->redirs = tmp->redirs->next;
			free(tmp2->content);
			free(tmp2);
		}
		while (tmp->args)
		{
			tmp2 = tmp->args;
			tmp->args = tmp->args->next;
			free(tmp2);
		}
		free(tmp);
	}
}
