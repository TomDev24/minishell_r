/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:39:06 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/07 18:32:41 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_token_by_addr(char *addr, t_token *tokens, int prev_flag)
{
	t_token	*res;
	t_token	*prev;
	t_token	*next;

	res = NULL;
	prev = NULL;
	while (tokens)
	{
		next = tokens->next;
		if (tokens->addr == addr)
		{
			if (prev_flag == 1)
				return (prev);
			if (prev_flag == 0)
				return (tokens);
			if (prev_flag == 2)
				return (next);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (res);
}

//points on last char of tkn->value in tkn->addr
char	*tkn_eof(t_token *tkn)
{
	int	i;

	i = 0;
	while (tkn->value[i] && tkn->addr[i] == tkn->value[i])
		i++;
	return (tkn->addr + i - 1);
}

int	cmdlst_size(t_cmd *cmds)
{
	int	i;

	i = 1;
	if (!cmds)
		return (0);
	while (cmds->next != NULL)
	{
		i++;
		cmds = cmds->next;
	}
	return (i);
}

int	array_size(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	**sort_array(char **s, int len_array)
{
	int		i;
	int		j;
	char	*tmp;
	int		offset;

	i = -1;
	j = 0;
	offset = 0;
	while (++i < len_array - 1)
	{
		while (++j < len_array)
		{
			while (s[i][offset] && s[j][offset] && s[i][offset] == s[j][offset])
				offset++;
			if (s[i][offset] && s[j][offset] && s[i][offset] > s[j][offset])
			{
				tmp = s[i];
				s[i] = s[j];
				s[j] = tmp;
			}
			offset = 0;
		}
		j = i + 1;
	}
	return (s);
}
