/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 19:18:33 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/06 19:49:42 by cgregory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//HASH_ENVP should always have alocated key and str
void	init_hash_envp(char **envp)
{
	t_ht	*ht;
	char	*key;
	char	*tmp;

	g_mshell.hash_envp = ht_create();
	ht = g_mshell.hash_envp;
	while (*envp)
	{
		//its very bad, think of something else
		key = *envp;
		tmp = ft_strchr(key, '=');
		*tmp = '\0';
		ht_set(ht, key, tmp + 1);
		*tmp = '=';
		envp++;
	}
}

/*
void	change_question(t_token *tokens){
	//char	*s;

	//s = ft_strchr(tokens->value, '$');	
	//if (!s)
	//	return;
	//printf("change\n");
	if (ft_strncmp(tokens->value, "$?", 3) == 0){
		printf("Changin token value to exit_code %s# ", tokens->value);
		free(tokens->value);
		tokens->value = ft_itoa(g_mshell.exit_code);
		printf("%s\n", tokens->value);
		while(g_mshell.cmds){
			if (tokens->i == g_mshell.cmds->i){
				while(*g_mshell.cmds->argv)
					printf("cmd_i %d %s ", g_mshell.cmds->i, *g_mshell.cmds->argv++);
			}
			g_mshell.cmds = g_mshell.cmds->next;
		}
	}	
}

void	update_g_mshell(int code, int cmd_i){
	t_token	*tokens;

	g_mshell.exit_code = code;
	tokens = g_mshell.tokens;
	//printf("UPDAT# MSHEL cmd-i %d\n", cmd_i);
	while (tokens){
		//printf("tkns-i %d\n", tokens->i);
		//if (tokens->i == cmd_i + 1)
		cmd_i++;
		change_question(tokens);
		//else if (tokens->i > cmd_i + 1)
			//break;
		tokens = tokens->next;
	}
}*/
