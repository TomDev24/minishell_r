/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 11:10:44 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/07 13:24:54 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*allocate_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		m_error(1);
	cmd->i = 0;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->eof = NULL;
	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

t_stack	*init_context(t_token **tokens)
{
	t_stack	*res;

	res = malloc(sizeof(t_stack));
	if (!res)
		m_error(1);
	res->evars_len = 0;
	res->q_type = -1;
	res->temp_type = '0';
	res->replace = 0;
	res->prev = NULL;
	res->next = NULL;
	res->elements = NULL;
	res->tokens = tokens;
	return (res);
}

//st_token=Q1/Q2/PREV
int	determine_type(t_token *st_token)
{
	if (st_token->type == ININ)
		return (DELIMITER);
	if (st_token->type == IN || st_token->type == OUT
		|| st_token->type == OUTOUT)
		return (FILEN);
	if ((st_token->type == Q1 || st_token->type == Q2))
		return (CMD);
	if (st_token->type == PIPE)
		return (CMD);
	return (ARG);
}

t_token	*create_replacer(t_token *st_token)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		m_error(1);
	new->type = determine_type(st_token);
	new->addr = NULL;
	new->next = NULL;
	return (new);
}

void	start_end_addr(t_stack *context, t_token **s_token, t_token **e_token)
{
	char		*str;
	t_token		**tokens;

	tokens = context->tokens;
	if (context->q_type != (*s_token)->type)
		*s_token = (*s_token)->next;
	str = (*s_token)->addr;
	while (str > (*tokens)->addr && *str != ' ')
		str--;
	if (*str == ' ')
		str++;
	*s_token = get_token_by_addr(str, *tokens, 0);
	if (*s_token)
	{
		context->prev = get_token_by_addr((*s_token)->addr, *tokens, 1);
		if (!context->prev)
			context->replace = 1;
	}	
	while ((*e_token)->next && (*e_token)->next->addr - tkn_eof(*e_token) == 1)
		*e_token = (*e_token)->next;
	context->next = (*e_token)->next;
}
