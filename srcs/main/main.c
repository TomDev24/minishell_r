/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:06:48 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/08 14:53:47 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_mshell;

void	sighandler_prepare(void)
{
	sigemptyset(&g_mshell.newset);
	sigaddset(&g_mshell.newset, SIGINT);
	sigaddset(&g_mshell.newset, SIGQUIT);
	g_mshell.s_int.sa_handler = sigint_handler;
	sigaction(SIGINT, &g_mshell.s_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	python_test(char *line, char **envp)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = NULL;
	cmds = NULL;
	tokens = lexer(line);
	cmds = parser(&tokens);
	executor(cmds, cmdlst_size(cmds));
	envp++;
}

static	void	minishell(char *line)
{
	t_token		*tokens;
	t_cmd		*cmds;

	tokens = NULL;
	cmds = NULL;
	add_history(line);
	tokens = lexer(line);
	pretty_lexer(tokens);
	cmds = parser(&tokens);
	executor(cmds, cmdlst_size(cmds));
	free_tokens(tokens);
	free_cmds(cmds);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;

	init_env(envp);
	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(argv[2], envp);
	set_param_tty();
	sighandler_prepare();
	while (1 && argc == 1)
	{
		line = readline("|sash>");
		if (line == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		minishell(line);
	}
	unset_param_tty();
	ht_delete(&g_mshell.env);
	return (0);
}
