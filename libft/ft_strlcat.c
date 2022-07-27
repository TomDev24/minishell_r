/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:50:24 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/06 17:01:37 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t len_src;
	size_t len_dst;

	len_src = ft_strlen(src);
	len_dst = ft_strlen(dst);
	if (dstsize == 0)
		return (len_src + (len_dst < dstsize ? len_dst : dstsize));
	if (dstsize <= len_dst)
		return (len_src + (len_dst < dstsize ? len_dst : dstsize));
	if (len_src < dstsize - len_dst)
		ft_memccpy(dst + len_dst, src, '\0', dstsize - 1);
	else
	{
		ft_memccpy(dst + len_dst, src, '\0', dstsize - len_dst - 1);
		dst[len_dst + dstsize - len_dst - 1] = '\0';
	}
	return (len_src + (len_dst < dstsize ? len_dst : dstsize));
}
