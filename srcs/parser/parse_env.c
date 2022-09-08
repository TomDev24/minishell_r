/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:46:52 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 13:23:29 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*try_replace_env(t_token *tokens, char *st_addr, char *value, int *i)
{
	t_token	*evar;
	int		j;

	j = 0;
	evar = get_token_by_addr(st_addr, tokens, 0);
	if (*evar->value)
	{
		while (evar->value[j])
		{
			value[*i] = evar->value[j++];
			*i = *i + 1;
		}
	}
	if (evar->end_addr)
		return (evar->end_addr);
	return (st_addr + 1);
}

void	merge_vars(t_token *first, t_token *current, t_stack *context)
{
	char	*value;
	char	*key;
	char	*tmp;

	tmp = NULL;
	key = current->value + 1;
	value = ht_get(g_mshell.env, key);
	if (*key == 0)
		value = "$";
	else if (ft_strncmp(key, "?", 2) == 0)
		value = ft_itoa(g_mshell.exit_code);
	free(current->value);
	current->value = "";
	if (!value)
		return ;
	if (context->q_type != -1)
		context->evars_len += ft_strlen(value);
	tmp = first->value;
	first->value = ft_strjoin(first->value, value);
	if (!first->value)
		m_error(1, "");
	if (tmp && *tmp)
		free(tmp);
}

void	change_token_value(t_token *current, t_stack *context)
{
	t_token	*first;
	t_token	*tmp_tkn;

	first = current;
	if (*current->addr != '$')
		current = current->next;
	while (current && *current->addr == '$')
	{
		if (first->end_addr && first->end_addr != current->addr)
			break ;
		first->end_addr = tkn_eof(current) + 1;
		merge_vars(first, current, context);
		tmp_tkn = current;
		current = current->next;
		if (first != tmp_tkn)
			free_tkn(&tmp_tkn);
	}
	first->next = current;
}

void	manage_evar(t_token *current, t_stack *context)
{
	if (current->type != CMD && current->type != ARG
		&& current->type != FILEN && current->type != DELIMITER)
		return ;
	else if (*current->addr != '$' && *(tkn_eof(current) + 1) != '$')
		return ;
	change_token_value(current, context);
}
