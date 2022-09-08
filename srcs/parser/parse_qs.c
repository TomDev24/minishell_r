/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_qs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:51:28 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/07 13:48:39 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_token(t_token *replacer, t_stack **context, t_token *current)
{
	t_token	*context_el;
	t_token	**tokens;

	tokens = (*context)->tokens;
	context_el = (*context)->elements->content;
	replacer->next = (*context)->next;
	if ((*context)->prev)
		(*context)->prev->next = replacer;
	else if ((*context)->replace == 1)
	{
		*tokens = replacer;
		replacer->type = CMD;
	}
	else if (context_el)
		context_el->next = replacer;
	else
		*tokens = replacer;
	current += 1 -1;
}

void	set_replacer_value(t_stack *c, t_token *s_t, t_token *e_t, char *value)
{
	int		i;
	char	*en_addr;
	char	*st_addr;

	i = 0;
	en_addr = tkn_eof(e_t);
	st_addr = s_t->addr;
	while (st_addr <= en_addr)
	{
		if (c->temp_type == '0' && (*st_addr == '"' || *st_addr == '\''))
			c->temp_type = *st_addr++;
		if (c->temp_type == '"' && *st_addr == '$')
		{
			st_addr = try_replace_env(*c->tokens, st_addr, value, &i);
			continue ;
		}
		if (*st_addr == c->temp_type)
		{
			c->temp_type = '0';
			st_addr++;
		}
		else
			value[i++] = *st_addr++;
	}
}

//SHOULD CHOSE CORRECT TYPE AND PARSE VALUE RIGHT
t_token	*resolve_context(t_stack *context, t_token *current)
{
	int		value_len;
	char	*value;
	t_token	*new;
	t_token	*st_token;
	t_token	*en_token;

	st_token = context->elements->content;
	en_token = current;
	new = create_replacer(st_token);
	start_end_addr(context, &st_token, &en_token);
	value_len = tkn_eof(en_token) - st_token->addr + 2 + context->evars_len;
	value = ft_calloc(value_len, sizeof(char));
	if (!value)
		m_error(1);
	set_replacer_value(context, st_token, en_token, value);
	new->value = value;
	free_context_elements(context, st_token, en_token);
	return (new);
}

//context LIST[PREV, Q1, ARG, ARG, Q2, Q1, NEXT]
//context LIST[PREV, Q1, ..., Q1,(nospace)NEXT...); should go as one context
t_token	*manage_context(t_token *current, t_token *prev, t_stack **context)
{
	t_token		*replacer;

	replacer = NULL;
	if ((*context)->q_type == current->type)
	{
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
		if ((*context)->next)
			ft_lstadd_back(&(*context)->elements, ft_lstnew((*context)->next));
		replacer = resolve_context(*context, current);
		replace_token(replacer, context, current);
		free_context(context);
		return (replacer);
	}
	else if ((*context)->q_type == -1
		&& (current->type == Q1 || current->type == Q2))
	{
		(*context)->q_type = current->type;
		if (prev)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(prev));
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	}
	else if ((*context)->q_type != -1)
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	return (current);
}

void	unquote(t_token **tokens)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;
	t_stack	*context;

	current = *tokens;
	context = NULL;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (context == NULL)
			context = init_context(tokens);
		context->next = next;
		manage_evar(current, context);
		current = manage_context(current, prev, &context);
		prev = current;
		current = current->next;
	}
	free_context(&context);
}
