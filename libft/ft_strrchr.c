/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 11:38:27 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/04 15:53:10 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char *start;
	char *res;

	res = 0;
	start = (char*)s;
	while (*s != '\0')
		s++;
	while (*s != (char)c && s != start)
		s--;
	if ((char)c == '\0' || *s == (char)c)
		res = (char*)s;
	return (res);
}
