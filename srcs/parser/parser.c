/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:23:44 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/07 11:42:20 by dbrittan         ###   ########.fr       */
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

char	**make_argv(t_cmd *cmd)
{
	char	**res;
	int		size;
	t_list	*tmp;
	t_token	*tkn;

	size = 1 + ft_lstsize(cmd->args);
	tmp = cmd->args;
	res = (char **)malloc(sizeof(res) * (size + 1));
	if (!res)
		m_error(1);
	*(res++) = cmd->cmd->value;
	while (tmp)
	{
		tkn = tmp->content;
		if (*tkn->value)
			*(res++) = tkn->value;
		else
			size--;
		tmp = tmp->next;
	}
	*res = NULL;
	return (res - size);
}

t_token	*save_redirection(t_token *tokens, t_cmd *new)
{
	t_redir	*redir;
	t_list	*item;

	if (!tokens->next)
		m_error(10);
	if (tokens->next->type == FILEN || tokens->next->type == DELIMITER)
	{
		redir = malloc(sizeof(t_redir));
		if (!redir)
			m_error(1);
		redir->type = tokens->type;
		redir->filen = tokens->next->value;
		item = ft_lstnew(redir);
		if (!item)
			m_error(1);
		ft_lstadd_back(&new->redirs, item);
	}
	return (tokens);
}

t_token	*pack_cmd(t_token *tokens, t_cmd **cmds)
{
	t_cmd		*new;
	t_list		*item;

	new = allocate_cmd();
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == CMD && !new->cmd)
			new->cmd = tokens;
		else if (tokens->type == ARG)
		{
			item = ft_lstnew(tokens);
			if (!item)
				m_error(1);
			ft_lstadd_back(&new->args, item);
		}
		else if (tokens->type >= OUT && tokens->type <= OUTOUT)
			tokens = save_redirection(tokens, new);
		tokens = tokens->next;
	}
	new->argv = make_argv(new);
	add_cmd_to_list(cmds, new);
	return (tokens);
}

t_cmd	*parser(t_token **tokens)
{
	t_cmd	*cmds;
	t_token	*tkns;

	cmds = NULL;
	unquote(tokens);
	tkns = *tokens;
	while (tkns)
	{
		tkns = pack_cmd(tkns, &cmds);
		if (tkns && tkns->type == PIPE)
			tkns = tkns->next;
	}
	return (cmds);
}
