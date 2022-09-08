/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 18:26:50 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 12:28:46 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect(t_redir *r, int std_in)
{
	int		fd;

	if (r->type == IN)
	{
		fd = open(r->filen, O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if (r->type == OUT)
	{
		fd = open(r->filen, O_TRUNC | O_CREAT | O_WRONLY, 0664);
		dup2(fd, 1);
		close(fd);
	}
	else if (r->type == OUTOUT)
	{
		fd = open(r->filen, O_APPEND | O_CREAT | O_WRONLY, 0664);
		dup2(fd, 1);
		close(fd);
	}
	else if (r->type == ININ)
	{
		dup2(std_in, 0);
		here_doc(r->filen);
	}
}

int	handle_redirects(t_cmd *cmd)
{
	t_redir	*r;
	t_list	*redirects;
	int		std_in;

	std_in = dup(0);
	redirects = cmd->redirs;
	while (redirects)
	{
		r = redirects->content;
		redirect(r, std_in);
		redirects = redirects->next;
	}
	return (1);
}

void	evaluate_and_write(char *line, int fd)
{
	t_token		*orig_tokens;
	t_token		*tokens;

	orig_tokens = lexer(line);
	unquote(&orig_tokens);
	tokens = orig_tokens;
	while (tokens)
	{
		write(fd, tokens->value, ft_strlen(tokens->value));
		tokens = tokens->next;
		if (tokens)
			write(fd, " ", 1);
	}
	free_tokens(orig_tokens);
}

void	here_doc(char *eof)
{
	int		fd;
	char	*line;

	line = "";
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		m_error(2);
	while (ft_strncmp(line, eof, ft_strlen(eof) + 1) != 0)
	{
		if (*line)
		{
			evaluate_and_write(line, fd);
			write(fd, "\n", 1);
			free(line);
		}
		line = readline(">");
	}
	free(line);
	close(fd);
	fd = open(".heredoc_tmp", O_RDONLY);
	dup2(fd, 0);
	close(fd);
	unlink(".heredoc_tmp");
}
