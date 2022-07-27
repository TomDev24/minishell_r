/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:27:14 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/06 16:47:34 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *h;

	if (!*lst)
		return ;
	while (*lst)
	{
		h = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = h;
	}
}
