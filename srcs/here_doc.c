#include "minishell.h"

/*
char	**make_argv2(){
	char	**res;
	int	size;
	t_list	*tmp;
	t_token	*tkn;

	size = 1 + ft_lstsize(cmd->args);
	tmp = cmd->args;
	res = (char **)malloc(sizeof(res) * (size + 1));
	if (!res)
		exit(1); // make better exit

	*(res++) = cmd->cmd->value;
	while (tmp){
		tkn = tmp->content;
		if (*tkn->value)
			*(res++) = tkn->value;
		else
			size--;
		tmp = tmp->next;
	}
	*res = NULL;
	return res - size;
}
*/

void	evaluate_and_write(char *line, int fd){
	t_token		*tokens;

	tokens = lexer(line);
	unquote(&tokens);
	while(tokens){
		write(fd, tokens->value, ft_strlen(tokens->value));
		write(fd, " ", 1);
		tokens = tokens->next;
	}
}

void	here_doc(char *eof){
	int	fd;
	char	*line;
			
	line = "";
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit(2);
	while (ft_strncmp(line, eof, ft_strlen(eof) + 1) != 0){
		if (*line){
			evaluate_and_write(line, fd);
			write(fd, "\n", 1);
		}
		line = readline(">");
	}
	close(fd);

	fd = open(".heredoc_tmp", O_RDONLY);
	dup2(fd, 0);
	close(fd);
	unlink(".heredoc_tmp");
}
