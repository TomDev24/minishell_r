/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 13:14:35 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/08 21:12:02 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char *word;
	unsigned char l;

	word = (unsigned char*)s;
	l = (unsigned char)c;
	while (n--)
	{
		if (*word++ == l)
			return (--word);
	}
	return (NULL);
}
