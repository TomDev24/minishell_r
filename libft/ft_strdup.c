/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 10:52:21 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/04 14:25:01 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*res;
	size_t	len;

	len = ft_strlen(s1);
	res = (char*)malloc(sizeof(*s1) * (len + 1));
	if (res)
		ft_strlcpy(res, s1, sizeof(*s1) * (len + 1));
	else
		errno = ENOMEM;
	return (res);
}
