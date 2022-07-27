/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 11:46:50 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/09 20:48:25 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*hay;
	size_t	n_len;

	if (!haystack && !needle)
		return (NULL);
	if (*needle == '\0')
		return (char*)haystack;
	n_len = ft_strlen(needle);
	hay = (char *)haystack;
	while (len-- && *hay)
	{
		if (n_len <= len + 1)
		{
			if ((ft_strncmp(hay, needle, n_len)) == 0)
				return (hay);
		}
		hay++;
	}
	return (NULL);
}
