/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:23:44 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/09 14:12:24 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_cmd_to_list(t_cmd **cmds, t_cmd *new)
{
	t_cmd	*tmp;
	int		i;

	i = 1;
	tmp = *cmds;
	new->i = 0;
	if (*cmds == NULL)
	{
		*cmds = new;
		return ;
	}
	while (tmp->next && ++i)
		tmp = tmp->next;
	new->i = i;
	tmp->next = new;
}

char	**make_argv(t_cmd *cmd, int size)
{
	char	**res;
	t_list	*tmp;
	t_token	*tkn;

	tmp = cmd->args;
	res = (char **)ft_calloc(size + 1, sizeof(res));
	if (!res)
		m_error(1, "");
	if (cmd->cmd)
		*(res++) = cmd->cmd->value;
	else
	{
		m_error(-7, "");
		return (res);
	}
	while (tmp && cmd->cmd)
	{
		tkn = tmp->content;
		if (tkn->value)
			*(res++) = tkn->value;
		else
			size--;
		tmp = tmp->next;
	}
	return (res - size);
}

t_token	*save_redirection(t_token *ts, t_cmd *new)
{
	t_redir	*redir;
	t_list	*item;

	if (!ts->next || (ts->next->type != FILEN && ts->next->type != DELIMITER))
	{
		m_error(-8, "");
		return (ts);
	}
	if (ts->next->type == FILEN || ts->next->type == DELIMITER)
	{
		redir = malloc(sizeof(t_redir));
		if (!redir)
			m_error(1, "");
		redir->type = ts->type;
		if (redir->type == IN || redir->type == ININ)
			new->infile = ts->next->value;
		if (redir->type == OUT || redir->type == OUTOUT)
			new->outfile = ts->next->value;
		redir->filen = ts->next->value;
		item = ft_lstnew(redir);
		if (!item)
			m_error(1, "");
		ft_lstadd_back(&new->redirs, item);
	}
	return (ts);
}

t_token	*pack_cmd(t_token *tokens, t_cmd **cmds)
{
	t_cmd		*new;
	t_list		*item;

	if (tokens->type == PIPE)
		return (tokens->next);
	new = allocate_cmd();
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == CMD && !new->cmd)
			new->cmd = tokens;
		else if (tokens->type == ARG)
		{
			item = ft_lstnew(tokens);
			if (!item)
				m_error(1, "");
			ft_lstadd_back(&new->args, item);
		}
		else if (tokens->type >= OUT && tokens->type <= OUTOUT)
			tokens = save_redirection(tokens, new);
		tokens = tokens->next;
	}
	new->argv = make_argv(new, 1 + ft_lstsize(new->args));
	add_cmd_to_list(cmds, new);
	return (tokens);
}

t_cmd	*parser(t_token **tokens)
{
	t_cmd	*cmds;
	t_token	*tkns;

	cmds = NULL;
	unquote(tokens);
	if (g_mshell.exit_code < 0)
		return (cmds);
	tkns = *tokens;
	while (tkns)
	{
		if (tkns && tkns->type == PIPE && !tkns->next)
		{
			m_error(-5, "");
			break ;
		}
		tkns = pack_cmd(tkns, &cmds);
		if (g_mshell.exit_code < 0)
			break ;
	}
	return (cmds);
}
