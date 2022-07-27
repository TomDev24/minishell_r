/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 13:24:20 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/05 17:02:49 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	convert(char c)
{
	return (c - '0');
}

int			ft_atoi(const char *str)
{
	int i;
	int sign;
	int res;

	res = 0;
	i = 0;
	sign = 1;
	while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t'
			|| str[i] == '\r' || str[i] == '\f' || str[i] == '\v')
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '+') ? 1 : -1;
	while (ft_isdigit(str[i]))
	{
		res *= 10;
		res += convert(str[i]);
		i++;
	}
	return (res * sign);
}
