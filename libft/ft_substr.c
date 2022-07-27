/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 11:17:09 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/06 16:23:21 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*res;

	i = start;
	if (!s)
		return (0);
	if (start > ft_strlen(s))
		len = 0;
	res = (char*)malloc(sizeof(*s) * (len + 1));
	if (res)
	{
		while (len-- && *s != '\0')
			*(res++) = s[i++];
		*res = '\0';
	}
	else
		errno = ENOMEM;
	return (res - (i - start));
}
