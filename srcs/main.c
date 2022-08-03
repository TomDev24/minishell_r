#include "minishell.h"

//FREE FUNCTIONS SHOULD REWRITTEN AND PLACED ELSEWHERE

char    **parse_envp(char **envp){
	char **res;
	int i = 0;

	res = NULL;
	while(envp[i]){
		if (ft_strncmp(envp[i], "PATH", 4) == 0){
			res = ft_split(ft_strchr(envp[i], '=') + 1, ':');
			break;
		}
		i++;
	}
	return res;
}

void	free_tokens(t_token *tokens){
	t_token	*tmp;

	while(tokens){
		tmp = tokens;
		tokens = tokens->next;
		free(tmp);
	}
}

void	free_arr(char **line){
	while(*line){
		free(*line);
		line++;
	}
	free(line);
}

void	free_cmds(t_cmd *cmds){
	//not best way
	t_cmd	*tmp;
	while(cmds){
		tmp = cmds;
		cmds = cmds->next;
		free(tmp->infile);
		free(tmp->outfile);
		//free_arr(tmp->argv);
		//ft_lstclear(
		free(tmp);
	}
}

void	python_test(t_token *tokens, t_cmd *cmds, char *line, char **envp){
	tokens = NULL;
	cmds = NULL;
	tokens = lexer(line);
	cmds = parser(tokens);	
	executor(cmds, envp);
}

t_global	mshell;

int	main(int argc, char **argv, char **envp){
	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;

	mshell.hash_envp = ht_create(); 
	init_hash_envp(envp);

	if (argc > 1 && ft_strncmp(argv[1], "-c", 2) == 0)
		python_test(tokens, cmds, argv[2], envp);

	while(1 && argc == 1){
		tokens = NULL;
		cmds = NULL;
		line = readline(">>");
		add_history(line);
		tokens = lexer(line);
		pretty_lexer(tokens);	

		cmds = parser(tokens);	
		//print_cmds(cmds);
		executor(cmds, envp);
		//free_tokens(tokens);
		//free_cmds(cmds);
	}
	return 0;
}
