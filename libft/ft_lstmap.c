/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 12:26:32 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/06 16:47:30 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list *res;
	t_list *h;

	res = NULL;
	h = res;
	if (!lst || !f)
		return (NULL);
	while (lst)
	{
		res = ft_lstnew(f(lst->content));
		if (!res)
		{
			ft_lstclear(&h, del);
			return (NULL);
		}
		ft_lstadd_back(&h, res);
		lst = lst->next;
	}
	return (h);
}
