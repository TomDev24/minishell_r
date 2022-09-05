#include "minishell.h"

int	tokens_push(t_token **tokens, int type, char *val, char *addr){
	t_token	*new;
	t_token *tmp;
	int	index;

	tmp = *tokens;
	index = 0;
	new = (t_token*)malloc(sizeof(t_token));
	if (!new)
		m_error(1);
	
	new->type = type;
	new->value = val;
	new->addr= addr; 
	new->i = index;
	new->end_addr = NULL; 
	new->next = NULL;
	if (*tokens == NULL){
		*tokens = new;
		return 1;
	}
	while (tmp->next){
		tmp = tmp->next;
		if(tmp->i >= index)
			index = tmp->i;
		tmp->i = index;
	}
	new->i = index;
	if (type == PIPE)
		new->i = index + 1;
	tmp->next = new;	
	return 1;
}

static	char *select_min(char *s){
	char *min;

	min = ft_strchr(s, '\0'); //actually the biggest
	if (ft_strchr(s, ' ') < min && ft_strchr(s, ' ') != NULL)
		min = ft_strchr(s, ' '); 
	if (ft_strchr(s, '"') < min && ft_strchr(s, '"') != NULL)
		min = ft_strchr(s, '"'); 
	if (ft_strchr(s, '\'') < min && ft_strchr(s, '\'') != NULL)
		min = ft_strchr(s, '\''); 
	if (ft_strchr(s, '$') < min && ft_strchr(s, '$') != NULL)
		min = ft_strchr(s, '$'); 

	return min;
}

int	is_char(char *s){
	int res;

	res = 0;
	if (ft_isprint(*s) || *s < 0)
		res=1;
	if (*s == '<' || *s == '>' || *s == '|' || *s =='"' || *s == '\'')
		res = 0;
	//if ()	res = 0; //should i cmp >> and << ?
	return res;	
}

int	inspect_string(char *line, int i, int type, t_token **tokens){
	char	*tmp; 
	char	*val;
	char	c;

	tmp = "";
	val = "";
	c = ' ';

	while(line[i] == ' ')
		i++;
	if (is_char(line+i)) { 
		tmp = select_min(line+i+1);
		c = *tmp;
		*tmp = '\0';
		val = ft_strdup(line + i);
		*tmp = c; 
		tokens_push(tokens, type, val, line + i);
		if (type == CMD || type == ARG){
			return inspect_string(line, i + ft_strlen(val), ARG, tokens);
		}
	}
	return (i + ft_strlen(val));
}

//we need to split $a$b$c on separate args
int	get_next_token(char *line, t_token **tokens){
	int	i;
	
	i = 0;
	if (line[i] == '\'' && ++i)
		return tokens_push(tokens, Q1, ft_strdup("'"), line + i - 1)
			* inspect_string(line, i, ARG, tokens);
	if (line[i] == '"' && ++i)
		return tokens_push(tokens, Q2, ft_strdup("\""), line + i - 1)
			* inspect_string(line, i, ARG, tokens);
	if (ft_strncmp(line+i, "<<", 2) == 0)
		return tokens_push(tokens, ININ, ft_strdup("<<"), line + i)
			* inspect_string(line, i + 2, DELIMITER, tokens);
	if (ft_strncmp(line+i, ">>", 2) == 0)
		return tokens_push(tokens, OUTOUT, ft_strdup(">>"), line + i)
			* inspect_string(line, i + 2, FILEN, tokens);
	if (line[i] == '>' && ++i)
		return tokens_push(tokens, OUT, ft_strdup(">"), line + i - 1)
			* inspect_string(line, i, FILEN, tokens);
	if (line[i] == '<' && ++i)
		return tokens_push(tokens, IN, ft_strdup("<"), line + i - 1)
			* inspect_string(line, i, FILEN, tokens);
	if (line[i] == '|')
		return (i+1) * tokens_push(tokens, PIPE, ft_strdup("|"), line + i);
	return inspect_string(line, i, CMD, tokens);
}

t_token	*lexer(char *line){
	int		i;
	t_token		*tokens; 

	i = 0;
	tokens = NULL;
	while(line[i]){
		while(line[i] == ' ')
			i++;
		i += get_next_token(line + i, &tokens);
	}
	return tokens;
}
