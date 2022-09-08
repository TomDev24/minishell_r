/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 10:51:16 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/08 13:42:39 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(int type, char *val, char *addr)
{
	t_token	*new;

	new = (t_token *) malloc(sizeof(t_token));
	if (!new)
		m_error(1, "");
	new->type = type;
	new->value = val;
	new->addr = addr;
	new->i = 0;
	new->end_addr = NULL;
	new->next = NULL;
	return (new);
}

char	*select_min(char *s)
{
	char	*min;

	min = ft_strchr(s, '\0');
	if (ft_strchr(s, ' ') < min && ft_strchr(s, ' ') != NULL)
		min = ft_strchr(s, ' ');
	if (ft_strchr(s, '"') < min && ft_strchr(s, '"') != NULL)
		min = ft_strchr(s, '"');
	if (ft_strchr(s, '\'') < min && ft_strchr(s, '\'') != NULL)
		min = ft_strchr(s, '\'');
	if (ft_strchr(s, '$') < min && ft_strchr(s, '$') != NULL)
		min = ft_strchr(s, '$');
	if (ft_strchr(s, '|') < min && ft_strchr(s, '|') != NULL)
		min = ft_strchr(s, '|');
	if (ft_strchr(s, '<') < min && ft_strchr(s, '<') != NULL)
		min = ft_strchr(s, '<');
	if (ft_strchr(s, '>') < min && ft_strchr(s, '>') != NULL)
		min = ft_strchr(s, '>');
	return (min);
}

int	is_char(char *s)
{
	int	res;

	res = 0;
	if (ft_isprint(*s) || *s < 0)
		res = 1;
	if (*s == '<' || *s == '>' || *s == '|' || *s == '"' || *s == '\'')
		res = 0;
	return (res);
}
