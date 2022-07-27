/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 11:48:08 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/06 21:52:16 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	setin(char const *set, char c, size_t *len)
{
	while (*set != '\0')
	{
		if (*set == c)
		{
			if (*len > 0)
				*len -= 1;
			return (1);
		}
		set++;
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	char	*from;
	char	*to;
	size_t	len;
	size_t	i;
	char	*res;

	i = 0;
	if (!s1 || !set)
		return (0);
	len = ft_strlen(s1);
	from = (char*)&s1[0];
	to = (char*)&s1[len];
	while (*(from) && setin(set, *from, &len))
		from++;
	if (from >= to)
		return (ft_strdup(""));
	while (*(to - 1) && setin(set, *(to - 1), &len))
		to--;
	res = (char*)ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (0);
	while (from != to && from < to)
		res[i++] = *from++;
	return (res);
}
