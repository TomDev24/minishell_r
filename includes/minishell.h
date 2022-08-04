#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>

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

typedef struct s_quotes{
	int		type;
	t_list		*q_list;

}			t_quotes;

typedef struct s_global{
	t_ht		*hash_envp;
	//char		**array_envp;
}			t_global;

extern	t_global	mshell;

/* LEXER */
int		tokens_push(t_token **tokens, int type, char *val, char *addr);
int		inspect_string(char *line, int i, int type, t_token **tokens);
int		get_next_token(char *line, t_token **tokens);
t_token		*lexer(char *line);

/* PARSER */
t_cmd		*parser(t_token *tokens);
char		**parse_envp(char **envp);

/* EXECUTOR */
void		executor(t_cmd *cmds, char **envp);

/* BUILTINS */
int		b_pwd();
int		b_echo(char **argv);
int		b_env();
int		b_export(char **argv);
int		b_unset(char **argv);

/* ENV	*/
void		init_hash_envp(char **envp);

/* UTILS */
void		python_test(char *line, char **envp);
int		cmdlst_size(t_cmd *cmds);
int		array_size(char **s);
char		**sort_array(char **s);

/* FREEING */
void		free_tokens(t_token *tokens);
void		free_arr(char **line);
void		free_cmds(t_cmd *cmds);

/* DEBUG */
void		print_cmds(t_cmd *cmds);
void		print_tokens(t_token *tokens);
void 		pretty_lexer(t_token *tokens);

#endif
