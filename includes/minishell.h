#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "signal.h"
#include "libft.h"

enum	lexer_types{
	CMD,
	ARG,
	PIPE,
	FILEN,
	OUT,
	IN,
	ININ,
	OUTOUT,
	DELIMITER,
	Q1,
	Q2
};

//addr is where token located in parsing line
typedef struct s_token{
	int		type;
	char		*value;
	char		*addr;
	struct s_token	*next;
}			t_token;

typedef struct s_cmd{
	int		i;
	char		*infile;
	char		*outfile;
	char		**argv;
	t_token		*cmd;
	struct s_cmd	*next;
}			t_cmd;

t_token		*lexer(char *line);
int		get_next_token(char *line, t_token **tokens);
int		tokens_push(t_token **tokens, int type, char *val, char *addr);

t_cmd		*parser(t_token *tokens);

/* DEBUG */
void		print_tokens(t_token *tokens);
void		print_cmds(t_cmd *cmds);
void 		pretty_lexer(t_token *tokens);

#endif
