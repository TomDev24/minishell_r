/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:11:41 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/06 21:54:20 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		append(char *to, unsigned int n)
{
	if (n + '0' >= '0' && n + '0' <= '9')
	{
		while (*to != '\0')
			to++;
		*to = n + '0';
	}
	else
	{
		append(to, n / 10);
		append(to, n % 10);
	}
}

static int		count_len(int n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		count++;
		n *= -1;
	}
	while (n / 10 != 0)
	{
		count++;
		n = n / 10;
	}
	return (++count);
}

char			*ft_itoa(int n)
{
	char *res;
	char *buf;

	res = (char*)ft_calloc(count_len(n) + 1, sizeof(char));
	buf = res;
	if (!res)
		return (0);
	if (n < 0)
	{
		n *= -1;
		*res++ = '-';
	}
	append(res, n);
	return (buf);
}
