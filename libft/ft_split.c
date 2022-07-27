/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 14:01:26 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/09 20:54:52 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	arr_len(size_t str_len, char const *s, char c)
{
	size_t i;
	size_t x;

	x = 0;
	i = 0;
	while (i < str_len - 1 && s[i] && s[i + 1])
	{
		if ((s[i] != c && i == 0) || (s[i] == c && s[i + 1] != c))
			x++;
		i++;
	}
	return (x);
}

static size_t	word_len(char const *str, size_t j, char c)
{
	size_t i;

	i = 0;
	while (str[j] && str[j] != c)
	{
		i++;
		j++;
	}
	return (i);
}

static void		free_mem(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i++] = NULL;
	}
	free(arr);
	arr = NULL;
}

static char		*al_str(char const *str, size_t j, char c, char **head)
{
	char	*res;
	size_t	i;
	size_t	len;

	i = 0;
	len = word_len(str, j, c);
	res = (char*)ft_calloc(len + 1, sizeof(char));
	if (!res)
	{
		free_mem(head);
		return (0);
	}
	while (str[j] && str[j] != c)
	{
		res[i] = str[j];
		i++;
		j++;
	}
	return (res);
}

char			**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	x;
	char	**res;
	char	**id;

	i = -1;
	id = NULL;
	if (!s)
		return (0);
	x = arr_len(ft_strlen(s), s, c);
	if (!(res = (char**)ft_calloc(x + 1, sizeof(res))))
		return (NULL);
	id = res;
	if (x == 0)
	{
		*res = (char*)'\0';
		return (res);
	}
	while (++i < ft_strlen(s) - 1 && id)
	{
		if ((s[i] != c && i == 0) || (s[i] == c && s[i + 1] != c))
			*res++ = al_str(s, (i == 0 && s[i] != c ? 0 : i + 1), c, id);
	}
	return (id);
}
