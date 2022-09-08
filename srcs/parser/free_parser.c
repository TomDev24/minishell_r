/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 13:24:04 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/07 13:58:59 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_context(t_stack **context)
{
	t_list	*elements;
	t_list	*tmp;
	t_stack	*ctx;

	if (!*context)
		return ;
	ctx = *context;
	elements = ctx->elements;
	while (elements)
	{
		tmp = elements;
		elements = elements->next;
		free(tmp);
	}
	free(*context);
	*context = NULL;
}

void	free_context_elements(t_stack *c, t_token *st_token, t_token *en_token)
{
	t_list	*head;
	t_token	*tmp;

	head = c->elements;
	if (head->next->content == st_token)
		head = head->next;
	while (st_token != en_token)
	{
		tmp = st_token;
		st_token = st_token->next;
		if (head && head->content == tmp)
		{
			free_tkn ((t_token **)&head->content);
			head = head->next;
		}
		else
			free_tkn (&tmp);
	}
	if (head && head->content == st_token)
		free_tkn ((t_token **)&head->content);
	else
		free_tkn (&st_token);
}
