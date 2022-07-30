#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/param.h>

#include "signal.h"
#include "libft.h"
#include "hash_t.h"

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
	t_list		*args;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_global{
	t_ht		*hash_envp;
	//char		**array_envp;
}		t_global;

extern	t_global	mshell;

t_token		*lexer(char *line);
int		get_next_token(char *line, t_token **tokens);
int		tokens_push(t_token **tokens, int type, char *val, char *addr);

t_cmd		*parser(t_token *tokens);
//delete this one bellow
char		**parse_envp(char **envp);

/* UTILS */
int		cmdlst_size(t_cmd *cmds);

/* DEBUG */
void		print_tokens(t_token *tokens);
void		print_cmds(t_cmd *cmds);
void 		pretty_lexer(t_token *tokens);

/* BUILTINS */
int		b_pwd();
int		b_echo(char **argv);
int		b_env();
int		b_export(char **argv);


void		init_hash_envp(char **envp);
#endif
