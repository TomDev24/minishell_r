/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tom <tom@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 20:02:56 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/09 11:31:49 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/param.h>
# include <sys/types.h>
# include <sys/wait.h>

# include "signal.h"
# include "libft.h"
# include "hash_t.h"
# include <termios.h>

//order matters
enum	e_lexer_types{
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

typedef struct termios	t_termios;

typedef struct s_token{
	int				i;
	int				type;
	char			*value;
	char			*addr;
	char			*end_addr;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd{
	int				i;
	char			*infile;
	char			*outfile;
	char			*eof;
	t_list			*redirs;
	char			**argv;
	t_token			*cmd;
	t_list			*args;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_redir{
	int			type;
	char		*filen;
}				t_redir;

typedef struct s_stack{
	int			evars_len;
	int			q_type;
	int			replace;
	char		temp_type;
	t_token		*prev;
	t_token		*next;
	t_list		*elements;
	t_token		**tokens;
}				t_stack;

typedef struct s_exec{
	int			**pipes;
	int			pipe_amount;
	int			*pids;
	t_list		*built_ins;
}				t_exec;

typedef struct s_global{
	int					exit_code;
	t_ht				*env;
	struct sigaction	s_int;
	struct sigaction	s_quit;
	sigset_t			newset;
	t_termios			setting_tty;
	t_termios			setting_out_tty;
}	t_global;

extern t_global			g_mshell;

void	set_param_tty(void);
void	unset_param_tty(void);
void	sigint_handler(int num);
void	sigquit_handler(int num);

/* ERROR */
int		handle_error_code(void);
void	m_error(int code, char *msg);

/* LEXER */
int		tokens_push(t_token **tokens, int type, char *val, char *addr);
int		inspect_string(char *line, int i, int type, t_token **tokens);
int		get_next_token(char *line, t_token **tokens);
t_token	*lexer(char *line);

/* UTILS_LEXER */
t_token	*init_token(int type, char *val, char *addr);
char	*select_min(char *s);
int		is_char(char *s);

/* PARSER */
t_cmd	*parser(t_token **tokens);

/* UTILS_PARSER */
t_cmd	*allocate_cmd(void);
t_stack	*init_context(t_token **tokens);
int		determine_type(t_token *st_token);
void	start_end_addr(t_stack *context, t_token **s_token, t_token **e_token);
t_token	*create_replacer(t_token *st_token);

/* FREE_PARSER */
void	free_context(t_stack **context);
void	free_context_elements(t_stack *c, t_token *st_token, t_token *en_token);

/* PARSE_QS*/
char	*tkn_eof(t_token *tkn);
void	unquote(t_token **tokens);

/* PARSE_ENV*/
char	*try_replace_env(t_token *tokens, char *st_addr, char *value, int *i);
void	change_token_value(t_token *current, t_stack *context);
void	manage_evar(t_token *current, t_stack *context);

/* EXECUTOR */
void	executor(t_cmd *cmds, int cmd_amount);

/* UTILS_EXECUTOR */
char	**parse_envp(void);
int		**init_pipes(int pipes_amount);
int		(*find_builtin(char *name))(char **value);
int		post_process(t_exec *exec, int cmd_amount);
void	pre_process(t_exec *exec, int cmd_amount);

/* REDIRECTS */
int		handle_redirects(t_cmd *cmd);
void	here_doc(char *eof);

/* BUILTINS */
int		b_pwd(char	**argv);
int		b_echo(char **argv);
int		b_env(char	**argv);
int		b_export(char **argv);
int		b_unset(char **argv);
int		b_exit(char **argv);
int		b_cd(char **argv);

/* ENV	*/
void	init_env(char **envp);
void	update_g_mshell(int code, int cmd_i);

/* UTILS */
t_token	*get_token_by_addr(char *addr, t_token *tokens, int prev);
char	*tkn_eof(t_token *tkn);
char	type_to_char(int Q);
void	python_test(char *line, char **envp);
int		cmdlst_size(t_cmd *cmds);
int		array_size(char **s);
char	**sort_array(char **s, int len_array);

/* FREEING */
void	free_tkn(t_token **tkn);
void	free_tokens(t_token *tokens);
void	free_arr(char **arr);
void	free_cmds(t_cmd *cmds);
void	free_pipes(int **pipes, int pipe_amount);
void	close_pipes(int	**pipes, int pipe_amount);

/* DEBUG */
char	*type_to_string(int type);
void	print_cmds(t_cmd *cmds);
void	print_tokens(t_token *tokens);
void	pretty_lexer(t_token *tokens);

#endif
