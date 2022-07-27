#include "minishell.h"

int	main(int argc, char **argv){
	t_token	*tokens;

	char *s1 = ft_strdup(argv[1]);
	tokens = lexer(s1);
	pretty_lexer(tokens);
	return 0;
}
