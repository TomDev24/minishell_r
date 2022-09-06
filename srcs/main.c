/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:06:48 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/06 19:52:39 by cgregory         ###   ########.fr       */
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
	g_mshell.s_int.sa_mask = SIGINT;
	sigaction(SIGINT, &g_mshell.s_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	init_hash_envp(envp);
	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(argv[2], envp);
	set_param_tty();
	sighandler_prepare();
	while (1 && argc == 1)
	{
		tokens = NULL;
		cmds = NULL;
		line = readline("sash>");
		if (line == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		add_history(line);
		tokens = lexer(line);
		g_mshell.tokens = tokens;
		//pretty_lexer(tokens);
		//print_tokens(tokens);
		cmds = parser(&tokens);
		g_mshell.cmds = cmds;
		print_cmds(cmds);
		executor(cmds);
		free_tokens(tokens);
		//free_cmds(cmds);
	}
	unset_param_tty();
	ht_delete(&g_mshell.hash_envp);
	return (0);
}
