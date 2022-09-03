#include "minishell.h"

void	evaluate_and_write(char *line, int fd){
	t_token		*orig_tokens;
	t_token		*tokens;

	orig_tokens = lexer(line);
	unquote(&orig_tokens);
	tokens = orig_tokens; 
	while(tokens){
		write(fd, tokens->value, ft_strlen(tokens->value));
		tokens = tokens->next;
		if (tokens)
			write(fd, " ", 1);
	}
	free_tokens(orig_tokens);
}

void	here_doc(char *eof){
	int	fd;
	char	*line;
			
	line = "";
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		m_error(2);
	while (ft_strncmp(line, eof, ft_strlen(eof) + 1) != 0){
		if (*line){
			evaluate_and_write(line, fd);
			write(fd, "\n", 1);
			//should i free line?
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
